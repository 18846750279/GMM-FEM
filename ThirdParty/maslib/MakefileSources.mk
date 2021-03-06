COMMON_SOURCES := \
    $(COMMONDIR)/mas/bvtree/bvtree.cxx \
    $(COMMONDIR)/mas/concurrency/thread.cxx \
    $(COMMONDIR)/mas/core/base.cxx \
    $(COMMONDIR)/mas/core/exception.cxx \
    $(COMMONDIR)/mas/core/math.cxx \
    $(COMMONDIR)/mas/core/time.cxx \
    $(COMMONDIR)/mas/csg/csg.cxx \
    $(COMMONDIR)/mas/fem/fem.cxx \
    $(COMMONDIR)/mas/mesh/io.cxx \
    $(COMMONDIR)/mas/mesh/mesh.cxx \
    $(COMMONDIR)/mas/mesh/meshbv.cxx \
    $(COMMONDIR)/mas/mesh/meshbv_intersector.cxx \
    $(COMMONDIR)/mas/mesh/simplification.cxx

MEX_SOURCES := \
    $(MEXDIR)/mas/bvtree/bvtree_build_mex.cxx \
    $(MEXDIR)/mas/bvtree/bvtree_destroy_mex.cxx \
    $(MEXDIR)/mas/bvtree/bvtree_intersect_plane_mex.cxx \
    $(MEXDIR)/mas/bvtree/bvtree_intersect_point_mex.cxx \
    $(MEXDIR)/mas/bvtree/bvtree_update_mex.cxx \
    $(MEXDIR)/mas/mesh/smesh_bvtree_build_mex.cxx \
    $(MEXDIR)/mas/mesh/smesh_bvtree_destroy_mex.cxx \
    $(MEXDIR)/mas/mesh/smesh_bvtree_is_inside_mex.cxx \
    $(MEXDIR)/mas/mesh/smesh_bvtree_nearest_polygon_mex.cxx

CMD_SOURCES := \
    $(CMDDIR)/mas/mesh/is_inside_cmd.cxx \
    $(CMDDIR)/mas/mesh/meshbv_cmd.cxx

TEST_SOURCES := \
    $(TESTDIR)/mas/bvtree/update_test.cxx \
    $(TESTDIR)/mas/concurrency/parallel_sort_test.cxx \
    $(TESTDIR)/mas/concurrency/queue_test.cxx \
    $(TESTDIR)/mas/concurrency/shared_lock_test.cxx \
    $(TESTDIR)/mas/concurrency/tuple_test.cxx \
    $(TESTDIR)/mas/core/heap_test.cxx \
    $(TESTDIR)/mas/core/martrix4d_test.cxx \
    $(TESTDIR)/mas/core/matrixnd_test.cxx \
    $(TESTDIR)/mas/core/qr_test.cxx \
    $(TESTDIR)/mas/core/queue_test.cxx \
    $(TESTDIR)/mas/mesh/simplification_test.cxx

M_SOURCES := \
    $(MEXDIR)/mas/bvtree/bvtree.m \
    $(MEXDIR)/mas/mesh/smesh_bvtree.m
