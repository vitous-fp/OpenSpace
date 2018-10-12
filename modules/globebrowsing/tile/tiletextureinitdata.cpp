/*****************************************************************************************
 *                                                                                       *
 * OpenSpace                                                                             *
 *                                                                                       *
 * Copyright (c) 2014-2018                                                               *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#include <modules/globebrowsing/tile/tiletextureinitdata.h>

namespace {

size_t numberOfRasters(ghoul::opengl::Texture::Format format) {
    switch (format) {
        case ghoul::opengl::Texture::Format::Red:
            return 1;
        case ghoul::opengl::Texture::Format::RG:
            return 2;
        case ghoul::opengl::Texture::Format::RGB:
        case ghoul::opengl::Texture::Format::BGR:
            return 3;
        case ghoul::opengl::Texture::Format::RGBA:
        case ghoul::opengl::Texture::Format::BGRA:
            return 4;
        default: {
            ghoul_assert(false, "Unknown format");
            throw ghoul::MissingCaseException();
        }
    }
}

size_t numberOfBytes(GLenum glType) {
    switch (glType) {
        case GL_UNSIGNED_BYTE:  return sizeof(GLubyte);
        case GL_BYTE:           return sizeof(GLbyte);
        case GL_UNSIGNED_SHORT: return sizeof(GLushort);
        case GL_SHORT:          return sizeof(GLshort);
        case GL_UNSIGNED_INT:   return sizeof(GLuint);
        case GL_INT:            return sizeof(GLint);
        case GL_HALF_FLOAT:     return sizeof(GLhalf);
        case GL_FLOAT:          return sizeof(GLfloat);
        case GL_DOUBLE:         return sizeof(GLdouble);
        default:
            ghoul_assert(false, "Unknown data type");
            throw ghoul::MissingCaseException();
    }
}

GLenum toGlTextureFormat(GLenum glType, ghoul::opengl::Texture::Format format) {
    switch (format) {
        case ghoul::opengl::Texture::Format::Red:
            switch (glType) {
                case GL_BYTE:
                    return GL_R8;
                case GL_UNSIGNED_BYTE:
                    return GL_R8;
                case GL_INT:
                    return GL_R32I;
                case GL_UNSIGNED_INT:
                    return GL_R32UI;
                case GL_FLOAT:
                    return GL_R32F;
                case GL_HALF_FLOAT:
                    return GL_R16F;
                default:
                    ghoul_assert(false, "glType data type unknown");
                    throw ghoul::MissingCaseException();
            }
        case ghoul::opengl::Texture::Format::RG:
            switch (glType) {
                case GL_BYTE:
                    return GL_RG8;
                case GL_UNSIGNED_BYTE:
                    return GL_RG8;
                case GL_INT:
                    return GL_RG32I;
                case GL_UNSIGNED_INT:
                    return GL_RG32UI;
                case GL_FLOAT:
                    return GL_RG32F;
                case GL_HALF_FLOAT:
                    return GL_RG16F;
                default:
                    ghoul_assert(false, "glType data type unknown");
                    throw ghoul::MissingCaseException();
            }
        case ghoul::opengl::Texture::Format::RGB:
            switch (glType) {
                case GL_BYTE:
                    return GL_RGB8;
                case GL_UNSIGNED_BYTE:
                    return GL_RGB8;
                case GL_INT:
                    return GL_RGB32I;
                case GL_UNSIGNED_INT:
                    return GL_RGB32UI;
                case GL_FLOAT:
                    return GL_RGB32F;
                case GL_HALF_FLOAT:
                    return GL_RGB16F;
                default:
                    ghoul_assert(false, "glType data type unknown");
                    throw ghoul::MissingCaseException();
            }
        case ghoul::opengl::Texture::Format::RGBA:
            switch (glType) {
                case GL_BYTE:
                    return GL_RGBA8;
                case GL_UNSIGNED_BYTE:
                    return GL_RGBA8;
                case GL_INT:
                    return GL_RGBA32I;
                case GL_UNSIGNED_INT:
                    return GL_RGBA32UI;
                case GL_FLOAT:
                    return GL_RGBA32F;
                case GL_HALF_FLOAT:
                    return GL_RGBA16F;
                default:
                    ghoul_assert(false, "glType data type unknown");
                    throw ghoul::MissingCaseException();
            }
        case ghoul::opengl::Texture::Format::BGR:
            switch (glType) {
                case GL_BYTE:
                    return GL_RGB8;
                case GL_UNSIGNED_BYTE:
                    return GL_RGB8;
                case GL_INT:
                    return GL_RGB32I;
                case GL_UNSIGNED_INT:
                    return GL_RGB32UI;
                case GL_FLOAT:
                    return GL_RGB32F;
                case GL_HALF_FLOAT:
                    return GL_RGB16F;
                default:
                    ghoul_assert(false, "glType data type unknown");
                    throw ghoul::MissingCaseException();
            }
        case ghoul::opengl::Texture::Format::BGRA:
            switch (glType) {
                case GL_BYTE:
                    return GL_RGBA8;
                case GL_UNSIGNED_BYTE:
                    return GL_RGBA8;
                case GL_INT:
                    return GL_RGBA32I;
                case GL_UNSIGNED_INT:
                    return GL_RGBA32UI;
                case GL_FLOAT:
                    return GL_RGBA32F;
                case GL_HALF_FLOAT:
                    return GL_RGBA16F;
                default:
                    ghoul_assert(false, "glType data type unknown");
                    throw ghoul::MissingCaseException();
            }
        default:
            ghoul_assert(false, "Unknown format for OpenGL texture");
            throw ghoul::MissingCaseException();
    }
}

} // namespace

namespace openspace::globebrowsing {

const glm::ivec2 TileTextureInitData::TilePixelStartOffset = glm::ivec2(-2);
const glm::ivec2 TileTextureInitData::TilePixelSizeDifference = glm::ivec2(4);

TileTextureInitData::TileTextureInitData(size_t width, size_t height, GLenum glType,
                                         Format textureFormat, PadTiles padTiles,
                                         ShouldAllocateDataOnCPU shouldAllocateDataOnCPU)
    : _glType(glType)
    , _ghoulTextureFormat(textureFormat)
    , _shouldAllocateDataOnCPU(shouldAllocateDataOnCPU)
    , _padTiles(padTiles)
{
    _tilePixelStartOffset = padTiles ? TilePixelStartOffset : glm::ivec2(0);
    _tilePixelSizeDifference = padTiles ? TilePixelSizeDifference : glm::ivec2(0);

    _dimensions = glm::ivec3(width, height, 1);
    _nRasters = numberOfRasters(_ghoulTextureFormat);
    _bytesPerDatum = numberOfBytes(glType);
    _bytesPerPixel = _nRasters * _bytesPerDatum;
    _bytesPerLine = _bytesPerPixel * _dimensions.x;
    _totalNumBytes = _bytesPerLine * _dimensions.y;
    _glTextureFormat = toGlTextureFormat(_glType, _ghoulTextureFormat);
    calculateHashKey();
}

TileTextureInitData::TileTextureInitData(const TileTextureInitData& original)
    : TileTextureInitData(
        original.dimensions().x,
        original.dimensions().y,
        original.glType(),
        original.ghoulTextureFormat(),
        PadTiles(original._padTiles),
        ShouldAllocateDataOnCPU(original.shouldAllocateDataOnCPU())
    )
{}

glm::ivec3 TileTextureInitData::dimensions() const {
    return _dimensions;
}

glm::ivec2 TileTextureInitData::tilePixelStartOffset() const {
    return _tilePixelStartOffset;
}

glm::ivec2 TileTextureInitData::tilePixelSizeDifference() const {
    return _tilePixelSizeDifference;
}

size_t TileTextureInitData::nRasters() const {
    return _nRasters;
}

size_t TileTextureInitData::bytesPerDatum() const {
    return _bytesPerDatum;
}

size_t TileTextureInitData::bytesPerPixel() const {
    return _bytesPerPixel;
}

size_t TileTextureInitData::bytesPerLine() const {
    return _bytesPerLine;
}

size_t TileTextureInitData::totalNumBytes() const {
    return _totalNumBytes;
}

GLenum TileTextureInitData::glType() const {
    return _glType;
}

TileTextureInitData::Format TileTextureInitData::ghoulTextureFormat() const {
    return _ghoulTextureFormat;
}

GLenum TileTextureInitData::glTextureFormat() const {
    return _glTextureFormat;
}

bool TileTextureInitData::shouldAllocateDataOnCPU() const {
    return _shouldAllocateDataOnCPU;
}

TileTextureInitData::HashKey TileTextureInitData::hashKey() const {
    return _hashKey;
}

void TileTextureInitData::calculateHashKey() {
    ghoul_assert(_dimensions.x > 0, "Incorrect dimension");
    ghoul_assert(_dimensions.y > 0, "Incorrect dimension");
    ghoul_assert(_dimensions.x <= 1024, "Incorrect dimension");
    ghoul_assert(_dimensions.y <= 1024, "Incorrect dimension");
    ghoul_assert(_dimensions.z == 1, "Incorrect dimension");
    unsigned int format = getUniqueIdFromTextureFormat(_ghoulTextureFormat);
    ghoul_assert(format < 256, "Incorrect format");

    _hashKey = 0LL;

    _hashKey |= _dimensions.x;
    _hashKey |= _dimensions.y << 10;
    _hashKey |= static_cast<std::underlying_type_t<GLenum>>(_glType) << (10 + 16);
    _hashKey |= format << (10 + 16 + 4);
}

unsigned int TileTextureInitData::getUniqueIdFromTextureFormat(
    Format textureFormat) const
{
    using Format = Format;
    switch (textureFormat) {
        case Format::Red:            return 0;
        case Format::RG:             return 1;
        case Format::RGB:            return 2;
        case Format::BGR:            return 3;
        case Format::RGBA:           return 4;
        case Format::BGRA:           return 5;
        case Format::DepthComponent: return 6;
        default:                     throw ghoul::MissingCaseException();
    }
}

} // namespace openspace::globebrowsing
