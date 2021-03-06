// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#include <noggit/blp_texture.h>

#include <cstdint>

#include <noggit/mpq/file.h>

#include <opengl/context.h>
#include <opengl/scoped.h>

#include <QGLPixelBuffer>

namespace noggit
{
  namespace detail
  {
#pragma pack (push, 1)
    struct blp_header
    {
      int32_t magix;
      int32_t version;
      uint8_t attr_0_compression;
      uint8_t attr_1_alphadepth;
      uint8_t attr_2_alphatype;
      uint8_t attr_3_mipmaplevels;
      int32_t resx;
      int32_t resy;
      int32_t offsets[16];
      int32_t sizes[16];
    };
#pragma pack (pop)
  }

  blp_texture::blp_texture (std::string const& filename)
    : blp_texture (QString::fromStdString (filename))
  {}

  blp_texture::blp_texture (const QString& filename)
  : texture()
  , _filename (filename)
  {
    bind();

    mpq::file f (_filename);

    const char* data (f.getPointer());
    const detail::blp_header* header(reinterpret_cast<const detail::blp_header*> (data));

    if (header->attr_0_compression == 1)
    {
      from_uncompressed_data (header, data);
    }
    else if (header->attr_0_compression == 2)
    {
      from_compressed_data(header, data);
    }

    gl.texParameteri ( GL_TEXTURE_2D
                    , GL_TEXTURE_MIN_FILTER
                    , GL_LINEAR_MIPMAP_LINEAR
                    );
    gl.texParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  const QString& blp_texture::filename()
  {
    return _filename;
  }

  void blp_texture::from_uncompressed_data ( const detail::blp_header* header
                                           , const char* data
                                           ) const
  {
    const unsigned int* palette
      ( reinterpret_cast<const unsigned int*> ( data
                                              + sizeof (detail::blp_header)
                                              )
      );

    size_t width (header->resx);
    size_t height (header->resy);

    unsigned int* result_buffer (new unsigned int[width * height]);

    const int alphabits (header->attr_1_alphadepth);

    // do every mipmap level
    for (size_t mipmap_level (0); mipmap_level < 16; ++mipmap_level)
    {
      width = std::max<size_t> (1, width);
      height = std::max<size_t> (1, height);

      if (!header->offsets[mipmap_level] || !header->sizes[mipmap_level])
      {
        break;
      }

      unsigned int* result_pointer (result_buffer);
      const unsigned char* color_pointer
        (reinterpret_cast<const unsigned char*>
          (&data[header->offsets[mipmap_level]]));

      if (alphabits != 0)
      {
        int alpha_bit_count (0);
        const unsigned char* alpha_pointer (color_pointer + width * height);

        for (size_t y (0); y < height; ++y)
        {
          for (size_t x (0); x < width; ++x)
          {
            unsigned int color_value (palette[*color_pointer++]);

            int alpha_value;

            if (alphabits == 8)
            {
              alpha_value = *alpha_pointer++;
            }
            else if (alphabits == 1)
            {
              alpha_value = (*alpha_pointer & (1 << alpha_bit_count))
                            ? 0xff
                            : 0x00;

              if (++alpha_bit_count == 8)
              {
                alpha_bit_count = 0;
                ++alpha_pointer;
              }
            }
            else
            {
              throw std::logic_error ("other alpha bit depths than 8 and 1 nyi");
            }

            *result_pointer++ = (color_value & 0x00FF0000) >> 16
                              | (color_value & 0x0000FF00)
                              | (color_value & 0x000000FF) << 16
                              | alpha_value << 24;
          }
        }
      }
      else
      {
        for (size_t y (0); y < height; ++y)
        {
          for (size_t x (0); x < width; ++x)
          {
            unsigned int color_value (palette[*color_pointer++]);
            *result_pointer++ = (color_value & 0x00FF0000) >> 16
                              | (color_value & 0x0000FF00)
                              | (color_value & 0x000000FF) << 16
                              | 0xFF000000;
          }
        }
      }

      gl.texImage2D ( GL_TEXTURE_2D
                   , mipmap_level
                   , GL_RGBA8
                   , width
                   , height
                   , 0
                   , GL_RGBA
                   , GL_UNSIGNED_BYTE
                   , result_buffer
                   );

      width >>= 1;
      height >>= 1;
    }

    delete[] result_buffer;
  }

  void blp_texture::from_compressed_data ( const detail::blp_header* header
                                         , const char* data
                                         ) const
  {
    //! \note 0 (0000) & 3 == 0, 1 (0001) & 3 == 1, 7 (0111) & 3 == 3
    static const GLint alphatypes[] = { GL_COMPRESSED_RGB_S3TC_DXT1_EXT
                                      , GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
                                      , 0
                                      , GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
                                      };
    static const size_t blocksizes[] = { 8
                                       , 16
                                       , 0
                                       , 16
                                       };

    const size_t alpha_type (header->attr_2_alphatype & 3);
    const size_t blocksize (blocksizes[alpha_type]);

    const GLint format ( alpha_type == 0
                       ? ( header->attr_1_alphadepth == 1
                         ? GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
                         : GL_COMPRESSED_RGB_S3TC_DXT1_EXT
                         )
                       : alphatypes[alpha_type]
                       );

    size_t width (header->resx);
    size_t height (header->resy);

    for (size_t mipmap_level (0); mipmap_level < 16; ++mipmap_level)
    {
      width = std::max<size_t> (1, width);
      height = std::max<size_t> (1, height);

      if (!header->offsets[mipmap_level] || !header->sizes[mipmap_level])
      {
        break;
      }

      gl.compressedTexImage2D ( GL_TEXTURE_2D
                             , mipmap_level
                             , format
                             , width
                             , height
                             , 0
                             , ((width + 3) / 4) * ((height + 3) / 4)* blocksize
                             , data + header->offsets[mipmap_level]
                             );

      width >>= 1;
      height >>= 1;
    }
  }

  QPixmap render_blp_to_pixmap ( const QString& blp_filename
                               , const int& width
                               , const int& height
                               )
  {
    QOpenGLContext context;
    context.create();

    QOpenGLFramebufferObjectFormat fmt;
    fmt.setSamples(1);
    fmt.setInternalTextureFormat(GL_RGBA8);

    QOffscreenSurface surface;
    surface.create();

    context.makeCurrent (&surface);

    opengl::context::scoped_setter const _ (::gl, &context);

    QOpenGLFramebufferObject pixel_buffer (width, height, fmt);
    pixel_buffer.bind();

    gl.viewport (0.0f, 0.0f, width, height);
    gl.matrixMode (GL_PROJECTION);
    gl.loadIdentity();
    gl.ortho (0.0f, width, height, 0.0f, 1.0f, -1.0f);
    gl.matrixMode (GL_MODELVIEW);
    gl.loadIdentity();

    gl.clearColor(.0f, .0f, .0f, .0f);
    gl.clear(GL_COLOR_BUFFER_BIT);

    opengl::scoped::texture_setter<0, GL_TRUE> const texture0;
    noggit::blp_texture const texture (blp_filename);

    gl.begin (GL_TRIANGLE_FAN);
    gl.texCoord2f (0.0f, 0.0f);
    gl.vertex2f (0.0f, 0.0f);
    gl.texCoord2f (1.0f, 0.0f);
    gl.vertex2f (width, 0.0f);
    gl.texCoord2f (1.0f, 1.0f);
    gl.vertex2f (width, height);
    gl.texCoord2f (0.0f, 1.0f);
    gl.vertex2f (0.0f, height);
    gl.end();

    QPixmap pixmap (QPixmap::fromImage (pixel_buffer.toImage()));

    if (pixmap.isNull())
    {
      throw std::runtime_error
        ("failed rendering " + blp_filename.toStdString() + " to pixmap");
    }
    return pixmap;
  }
}
