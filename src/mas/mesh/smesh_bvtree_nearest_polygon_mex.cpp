#include "mas/bvtree/bvtree.h"
#include "mex.h"
#include "mas/mesh/meshbv.h"
#include "mas/mesh/smesh_bvtree_mex.h"
#include "mas/mexhandle/mexhandle.h"
#include <math.h>

#define TREE_IDX 0
#define PNTS_IDX 1

#define IDXS_OUT 0
#define NEAREST_OUT 1

#define DIM 3

using namespace mas::bvtree;

 // Main entry function
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {


    if (nrhs < 1) {
        mexErrMsgIdAndTxt( "MATLAB:bvtree_intersect_point_mex:invalidNumInputs",
                        "Must have at least 1 input.");
    }
    if (nlhs > 2){
        mexErrMsgIdAndTxt( "MATLAB:bvtree_destroy_mex:maxlhs",
                "Too many output arguments.");
    }

    // Get data
    PBVTree tree = NULL;
    if (nrhs > TREE_IDX) {
    	mex::class_holder<PBVTree> *treeHolder = mex::get_class_holder<PBVTree>(MESH_TREE_SIGNATURE, prhs[TREE_IDX]);
    	tree = treeHolder->getData();

    	if (tree == NULL) {
    		mexPrintf("Unable to recover tree");
    	}

    	if (tree == NULL) {
    		mexErrMsgIdAndTxt( "MATLAB:bvtree_intersect_point_mex:invalidInput",
    		            "Cannot find BVTree with supplied id.");
    	}

    } else {
        mexErrMsgIdAndTxt( "MATLAB:bvtree_intersect_point_mex:invalidInputType",
            "Expecting an integer id.");
    }

    // get Points
    // Get data
    double *pnts;
    int nPoints = 0;
    if (nrhs > PNTS_IDX && !mxIsEmpty(prhs[PNTS_IDX]) && mxIsDouble(prhs[PNTS_IDX])) {
    	pnts = mxGetPr(prhs[PNTS_IDX]);
    	nPoints = mxGetN(prhs[PNTS_IDX]);

    	int dim = mxGetM(prhs[PNTS_IDX]);
    	if (dim != DIM) {
    		mexErrMsgIdAndTxt( "MATLAB:bvtree_mex:invalidInput",
    				"Point array must be of size 3xN.");
    	}
    } else {
    	mexErrMsgIdAndTxt( "MATLAB:bvtree_mex:invalidInputType",
    			"Point array must be of type double.");
    }

    // build output
    mxArray* idxsArray = mxCreateDoubleMatrix(nPoints, 1, mxREAL);
    double *idxs = mxGetPr(idxsArray);
    mxArray* nearestArray = mxCreateDoubleMatrix(DIM, nPoints, mxREAL);
    double *nearest = mxGetPr(nearestArray);

	// #pragma omp parallel for
    for (int i=0; i<nPoints; i++) {
   		mas::Point3d pnt(pnts[3*i], pnts[3*i+1], pnts[3*i+2]);

   		mas::Point3d np;

   		// find nearest face
   		mas::mesh::PBoundablePolygon bpoly = mas::mesh::nearest_polygon(pnt, tree, np);

   		nearest[3*i] = np.x;
   		nearest[3*i+1] = np.y;
   		nearest[3*i+2] = np.z;
   		idxs[i] = bpoly->getIndex();
    }

    if (nlhs > IDXS_OUT) {
    	plhs[IDXS_OUT] = idxsArray;
    }

    if (nlhs > NEAREST_OUT) {
    	plhs[NEAREST_OUT] = nearestArray;
    }



}
