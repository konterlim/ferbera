#include "../mock/config_mock.h"
#include "metadata/ffmpeg_thumbnailer_handler.h"
#include <metadata/ffmpeg_handler.h>

#include <gtest/gtest.h>

#ifdef HAVE_FFMPEGTHUMBNAILER

using ::testing::Return;

TEST(Thumbnailer_Cache, BaseDirFromConfig)
{
    auto ctx = std::make_shared<Context>(std::make_shared<ConfigMock>(), nullptr, nullptr, nullptr, nullptr);
    auto cfg = std::static_pointer_cast<ConfigMock>(ctx->getConfig());
    EXPECT_CALL(*cfg, getOption(CFG_SERVER_EXTOPTS_FFMPEGTHUMBNAILER_CACHE_DIR))
        .WillOnce(Return("/var/lib/cache"));
    EXPECT_EQ(FfmpegThumbnailerHandler(ctx, CFG_SERVER_EXTOPTS_FFMPEGTHUMBNAILER_ENABLED).getThumbnailCacheBasePath(), fs::path { "/var/lib/cache" });
}

TEST(Thumbnailer_Cache, BaseDirDefaultFromUserHome)
{
    auto ctx = std::make_shared<Context>(std::make_shared<ConfigMock>(), nullptr, nullptr, nullptr, nullptr);
    auto cfg = std::static_pointer_cast<ConfigMock>(ctx->getConfig());
    EXPECT_CALL(*cfg, getOption(CFG_SERVER_EXTOPTS_FFMPEGTHUMBNAILER_CACHE_DIR))
        .WillOnce(Return(""));
    EXPECT_CALL(*cfg, getOption(CFG_SERVER_HOME))
        .WillOnce(Return("/var/lib/gerbera"));
    EXPECT_EQ(FfmpegThumbnailerHandler(ctx, CFG_SERVER_EXTOPTS_FFMPEGTHUMBNAILER_ENABLED).getThumbnailCacheBasePath(), fs::path { "/var/lib/gerbera/cache-dir" });
}

TEST(Thumbnailer_Cache, CachePathAppendsAbsolute)
{
    auto result = FfmpegThumbnailerHandler::getThumbnailCachePath("/data/cache", "/data/video/file.avi");
    EXPECT_EQ(result, fs::path { "/data/cache/data/video/file.avi-thumb.jpg" });
}

TEST(Thumbnailer_Cache, CacheUniquePaths)
{
    const auto cacheBase = fs::path { "/database/cache" };
    // 2 similar paths with same file name.
    auto path1 = FfmpegThumbnailerHandler::getThumbnailCachePath(cacheBase, "/database/images/2020/04/image-1.jpg");
    auto path2 = FfmpegThumbnailerHandler::getThumbnailCachePath(cacheBase, "/database/images/2020/05/image-1.jpg");
    EXPECT_NE(path1, path2);
}

#endif
