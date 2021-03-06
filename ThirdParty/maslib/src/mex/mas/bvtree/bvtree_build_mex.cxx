#include "mas/bvtree/bvtree.h"
#include "mas/bvtree/bvtree_mex.h"
#include "mex.h"
#include "mas/mexhandle/mexhandle.h"
#include <math.h>

#define PNTS_IDX 0
#define GROUP_IDX 1
#define TOL_IDX 2

#define TREE_IDX 0

#define DIM 3

#include <memory>

using namespace mas::bvtree;

// Main entry function
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

    if (nrhs < 2) {
        mexErrMsgIdAndTxt("MATLAB:bvtree_build:invalidNumInputs",
                "Must have at least 2 inputs.");
    }
    if (nlhs > 1) {
        mexErrMsgIdAndTxt("MATLAB:bvtree_build:maxlhs",
                "Too many output arguments.");
    }

    // Create shared indexed points
    using SharedPoint = std::shared_ptr<mas::IndexedPoint3d>;
    using BoundablePoints = mas::bvtree::BoundablePointPtrSet<SharedPoint>;
    using SharedBoundablePoints = std::shared_ptr<BoundablePoints>;
    using AABBTree = mas::bvtree::BVTree<SharedBoundablePoints, AABB>;

    std::vector<SharedPoint> points;

    // Get point data
    if (nrhs > PNTS_IDX && mxIsDouble(prhs[PNTS_IDX])) {
        double *pnts = mxGetPr(prhs[PNTS_IDX]);
        int nPnts = mxGetN(prhs[PNTS_IDX]);
        points.reserve(nPnts);

        int dim = mxGetM(prhs[PNTS_IDX]);
        if (dim != DIM) {
            mexErrMsgIdAndTxt("MATLAB:bvtree_build:invalidInput",
                    "Point array must be of size 3xN.");
        }

        for (int i = 0; i < nPnts; i++) {
            points.push_back(
                    std::make_shared<mas::IndexedPoint3d>(pnts[3 * i],
                            pnts[3 * i + 1], pnts[3 * i + 2], i));
        }

    } else {
        mexErrMsgIdAndTxt("MATLAB:bvtree_build:invalidInputType",
                "Point array must be of type double.");
    }

    std::vector<SharedBoundablePoints> boundableGroups;
    if (nrhs > GROUP_IDX) {

        if (mxIsCell(prhs[GROUP_IDX])) {

            double *p;
            mxArray *cellElement;
            int N = mxGetNumberOfElements(prhs[GROUP_IDX]);

            int idx = 0;	// start at index 0
            for (int i = 0; i < N; i++) {
                cellElement = mxGetCell(prhs[GROUP_IDX], i);
                p = mxGetPr(cellElement);
                int M = mxGetNumberOfElements(cellElement);

                std::vector<SharedPoint> pointset;
                for (int m = 0; m < M; m++) {
                    pointset.push_back(points[(int) p[m] - 1]); // -1 for matlab indexing, copying in case part of more than one boundable
                }

                // move unique boundable into vector
                boundableGroups.push_back(
                        std::make_shared<BoundablePoints>(std::move(pointset), idx++));
            }

        } else if (mxIsDouble(prhs[GROUP_IDX])) {

            double *dgroup = mxGetPr(prhs[GROUP_IDX]);
            int M = mxGetM(prhs[GROUP_IDX]);
            int N = mxGetN(prhs[GROUP_IDX]);

            int idx = 0; // start at index 0

            for (int n = 0; n < N; n++) {

                std::vector<SharedPoint> pointset;
                for (int m = 0; m < M; m++) {
                    pointset.push_back(points[(int) dgroup[n * M + m] - 1]);
                }

                // movie unique boundable into vector
                boundableGroups.push_back(
                        std::make_shared<BoundablePoints>(std::move(pointset), idx++));
            }
        } else {
            mexErrMsgIdAndTxt("MATLAB:bvtree_build:invalidInputType",
                    "Groups of points must be an array or cell array of doubles (with integer values).");
        }
    }

    double tol = -1;
    if (nrhs > TOL_IDX && !mxIsEmpty(prhs[TOL_IDX])
            && mxIsDouble(prhs[TOL_IDX])) {
        double *eptr = mxGetPr(prhs[TOL_IDX]);
        tol = eptr[0];
    }

    if (tol < 0) {
        mas::Point3d sum(0, 0, 0);
        for (SharedPoint& pnt : points) {
            sum.add(*pnt);
        }
        sum.scale(1.0 / points.size());
        double r = 0;
        for (SharedPoint& pnt : points) {
            double d = pnt->distance(sum);
            if (d > r) {
                r = d;
            }
        }

        tol = r * 1e-12;
    }

    // construct the actual BVTree using an AABB as a base
    mex::class_handle<AABBTree> *tree = new mex::class_handle<AABBTree>(
            POINTSET_TREE_SIGNATURE, std::move(boundableGroups), tol);

    // return tree
    if (nlhs > TREE_IDX) {
        plhs[TREE_IDX] = mex::get_mex_handle<AABBTree>(tree);
    }

}
