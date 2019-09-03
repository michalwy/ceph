add_library(rbd_types STATIC
  journal/Types.cc
  mirroring_watcher/Types.cc
  trash_watcher/Types.cc
  watcher/Types.cc
  WatchNotifyTypes.cc)

set(librbd_internal_srcs
  AsyncObjectThrottle.cc
  AsyncRequest.cc
  DeepCopyRequest.cc
  ExclusiveLock.cc
  ImageCtx.cc
  ImageState.cc
  ImageWatcher.cc
  internal.cc
  Journal.cc
  LibrbdAdminSocketHook.cc
  ManagedLock.cc
  MirroringWatcher.cc
  ObjectMap.cc
  Operations.cc
  TrashWatcher.cc
  Utils.cc
  Watcher.cc
  api/Config.cc
  api/DiffIterate.cc
  api/Group.cc
  api/Image.cc
  api/Migration.cc
  api/Mirror.cc
  api/Namespace.cc
  api/Pool.cc
  api/PoolMetadata.cc
  api/Snapshot.cc
  api/Trash.cc
  cache/ImageWriteback.cc
  cache/ObjectCacherObjectDispatch.cc
  cache/ObjectCacherWriteback.cc
  cache/PassthroughImageCache.cc
  cache/WriteAroundObjectDispatch.cc
  deep_copy/ImageCopyRequest.cc
  deep_copy/MetadataCopyRequest.cc
  deep_copy/ObjectCopyRequest.cc
  deep_copy/SetHeadRequest.cc
  deep_copy/SnapshotCopyRequest.cc
  deep_copy/SnapshotCreateRequest.cc
  deep_copy/Utils.cc
  exclusive_lock/AutomaticPolicy.cc
  exclusive_lock/PreAcquireRequest.cc
  exclusive_lock/PostAcquireRequest.cc
  exclusive_lock/PreReleaseRequest.cc
  exclusive_lock/StandardPolicy.cc
  image/AttachChildRequest.cc
  image/AttachParentRequest.cc
  image/CloneRequest.cc
  image/CloseRequest.cc
  image/CreateRequest.cc
  image/DetachChildRequest.cc
  image/DetachParentRequest.cc
  image/ListWatchersRequest.cc
  image/OpenRequest.cc
  image/PreRemoveRequest.cc
  image/RefreshParentRequest.cc
  image/RefreshRequest.cc
  image/RemoveRequest.cc
  image/SetFlagsRequest.cc
  image/SetSnapRequest.cc
  image/ValidatePoolRequest.cc
  image_watcher/NotifyLockOwner.cc
  io/AioCompletion.cc
  io/AsyncOperation.cc
  io/CopyupRequest.cc
  io/ImageDispatchSpec.cc
  io/ImageRequest.cc
  io/ImageRequestWQ.cc
  io/ObjectDispatch.cc
  io/ObjectDispatchSpec.cc
  io/ObjectDispatcher.cc
  io/ObjectRequest.cc
  io/ReadResult.cc
  io/SimpleSchedulerObjectDispatch.cc
  io/Utils.cc
  journal/CreateRequest.cc
  journal/DemoteRequest.cc
  journal/ObjectDispatch.cc
  journal/OpenRequest.cc
  journal/PromoteRequest.cc
  journal/RemoveRequest.cc
  journal/Replay.cc
  journal/ResetRequest.cc
  journal/StandardPolicy.cc
  journal/Utils.cc
  managed_lock/AcquireRequest.cc
  managed_lock/BreakRequest.cc
  managed_lock/GetLockerRequest.cc
  managed_lock/ReacquireRequest.cc
  managed_lock/ReleaseRequest.cc
  managed_lock/Utils.cc
  mirror/DemoteRequest.cc
  mirror/DisableRequest.cc
  mirror/EnableRequest.cc
  mirror/GetInfoRequest.cc
  mirror/GetStatusRequest.cc
  mirror/PromoteRequest.cc
  object_map/CreateRequest.cc
  object_map/InvalidateRequest.cc
  object_map/LockRequest.cc
  object_map/RefreshRequest.cc
  object_map/RemoveRequest.cc
  object_map/Request.cc
  object_map/ResizeRequest.cc
  object_map/SnapshotCreateRequest.cc
  object_map/SnapshotRemoveRequest.cc
  object_map/SnapshotRollbackRequest.cc
  object_map/UnlockRequest.cc
  object_map/UpdateRequest.cc
  operation/DisableFeaturesRequest.cc
  operation/EnableFeaturesRequest.cc
  operation/FlattenRequest.cc
  operation/MetadataRemoveRequest.cc
  operation/MetadataSetRequest.cc
  operation/MigrateRequest.cc
  operation/ObjectMapIterate.cc
  operation/RebuildObjectMapRequest.cc
  operation/RenameRequest.cc
  operation/Request.cc
  operation/ResizeRequest.cc
  operation/SnapshotCreateRequest.cc
  operation/SnapshotProtectRequest.cc
  operation/SnapshotRemoveRequest.cc
  operation/SnapshotRenameRequest.cc
  operation/SnapshotRollbackRequest.cc
  operation/SnapshotUnprotectRequest.cc
  operation/SnapshotLimitRequest.cc
  operation/SparsifyRequest.cc
  operation/TrimRequest.cc
  trash/MoveRequest.cc
  trash/RemoveRequest.cc
  watcher/Notifier.cc
  watcher/RewatchRequest.cc
  ${CMAKE_SOURCE_DIR}/src/common/ContextCompletion.cc)

set(librbd_ocf_srcs
  cache/src/ocf/cleaning/acp.c
  cache/src/ocf/cleaning/alru.c
  cache/src/ocf/cleaning/cleaning.c
  cache/src/ocf/cleaning/nop.c
  cache/src/ocf/concurrency/ocf_cache_concurrency.c
  cache/src/ocf/concurrency/ocf_concurrency.c
  cache/src/ocf/engine/cache_engine.c
  cache/src/ocf/engine/engine_bf.c
  cache/src/ocf/engine/engine_common.c
  cache/src/ocf/engine/engine_d2c.c
  cache/src/ocf/engine/engine_discard.c
  cache/src/ocf/engine/engine_fast.c
  cache/src/ocf/engine/engine_inv.c
  cache/src/ocf/engine/engine_ops.c
  cache/src/ocf/engine/engine_pt.c
  cache/src/ocf/engine/engine_rd.c
  cache/src/ocf/engine/engine_wa.c
  cache/src/ocf/engine/engine_wb.c
  cache/src/ocf/engine/engine_wi.c
  cache/src/ocf/engine/engine_wt.c
  cache/src/ocf/engine/engine_zero.c
  cache/src/ocf/env/ocf_env.c
  cache/src/ocf/eviction/eviction.c
  cache/src/ocf/eviction/lru.c
  cache/src/ocf/metadata/metadata.c
  cache/src/ocf/metadata/metadata_collision.c
  cache/src/ocf/metadata/metadata_hash.c
  cache/src/ocf/metadata/metadata_io.c
  cache/src/ocf/metadata/metadata_misc.c
  cache/src/ocf/metadata/metadata_partition.c
  cache/src/ocf/metadata/metadata_raw_atomic.c
  cache/src/ocf/metadata/metadata_raw.c
  cache/src/ocf/metadata/metadata_raw_dynamic.c
  cache/src/ocf/metadata/metadata_raw_volatile.c
  cache/src/ocf/metadata/metadata_updater.c
  cache/src/ocf/mngt/ocf_mngt_cache.c
  cache/src/ocf/mngt/ocf_mngt_common.c
  cache/src/ocf/mngt/ocf_mngt_core.c
  cache/src/ocf/mngt/ocf_mngt_core_pool.c
  cache/src/ocf/mngt/ocf_mngt_flush.c
  cache/src/ocf/mngt/ocf_mngt_io_class.c
  cache/src/ocf/mngt/ocf_mngt_misc.c
  cache/src/ocf/ocf_cache.c
  cache/src/ocf/ocf_core.c
  cache/src/ocf/ocf_ctx.c
  cache/src/ocf/ocf_io.c
  cache/src/ocf/ocf_io_class.c
  cache/src/ocf/ocf_logger.c
  cache/src/ocf/ocf_metadata.c
  cache/src/ocf/ocf_queue.c
  cache/src/ocf/ocf_request.c
  cache/src/ocf/ocf_stats_builder.c
  cache/src/ocf/ocf_stats.c
  cache/src/ocf/ocf_trace.c
  cache/src/ocf/ocf_volume.c
  cache/src/ocf/utils/utils_async_lock.c
  cache/src/ocf/utils/utils_cache_line.c
  cache/src/ocf/utils/utils_cleaner.c
  cache/src/ocf/utils/utils_io.c
  cache/src/ocf/utils/utils_list.c
  cache/src/ocf/utils/utils_part.c
  cache/src/ocf/utils/utils_pipeline.c
  cache/src/ocf/utils/utils_realloc.c
  cache/src/ocf/utils/utils_refcnt.c
  cache/ocf/Ctx.cc
  cache/ocf/VolumeType.cc
  cache/ocf/Volume.cc
  cache/ocf/AIOVolume.cc
  cache/ocf/RBDVolume.cc
  cache/ocf/CacheCtx.cc
  cache/ocf/CachedVolumeCtx.cc
)

add_library(rbd_api STATIC librbd.cc)
add_library(rbd_ocf STATIC ${librbd_ocf_srcs})
add_library(rbd_internal STATIC
  ${librbd_internal_srcs}
  $<TARGET_OBJECTS:rados_snap_set_diff_obj>)
if(WITH_LTTNG)
  # librbd.cc includes tracing/librbd.h
  add_dependencies(rbd_api librbd-tp)
  # io/AioCompletion.cc includes tracing/librbd.h
  add_dependencies(rbd_internal librbd-tp)
endif()
if(WITH_LTTNG AND WITH_EVENTTRACE)
  add_dependencies(rbd_internal eventtrace_tp)
endif()
target_link_libraries(rbd_internal PRIVATE
  osdc)
target_include_directories(rbd_ocf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cache/include)
target_include_directories(rbd_ocf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cache/src/ocf/env)
target_compile_options(rbd_ocf PRIVATE -Wno-pointer-arith -Wno-type-limits)

add_library(librbd ${CEPH_SHARED}
  librbd.cc)
if(WITH_LTTNG)
  add_dependencies(librbd librbd-tp)
endif()

target_link_libraries(librbd PRIVATE aio)

target_link_libraries(librbd PRIVATE
  rbd_internal
  rbd_ocf
  rbd_types
  journal
  librados
  cls_rbd_client 
  cls_lock_client 
  cls_journal_client 
  ceph-common
  pthread
  ${CMAKE_DL_LIBS}
  ${EXTRALIBS} ${GSSAPI_LIBRARIES})
if(HAVE_UDEV)
  target_link_libraries(librbd PRIVATE
    udev)
endif()
if(ENABLE_SHARED)
  set_target_properties(librbd PROPERTIES
    OUTPUT_NAME rbd
    VERSION 1.12.0
    SOVERSION 1
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON)
    if(NOT APPLE)
      set_property(TARGET librbd APPEND_STRING PROPERTY
        LINK_FLAGS " -Wl,--exclude-libs,ALL")
    endif()
endif(ENABLE_SHARED)
install(TARGETS librbd DESTINATION ${CMAKE_INSTALL_LIBDIR})
