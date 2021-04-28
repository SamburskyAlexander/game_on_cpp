#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>


Image::Image(const std::string &a_path)
{
  if((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, 0)) != nullptr)
  {
    size = width * height * channels;
  }
}

Image::Image(int a_width, int a_height, int a_channels)
{
  data = new Pixel[a_width * a_height ];

  if(data != nullptr)
  {
    width = a_width;
    height = a_height;
    size = a_width * a_height * a_channels;
    channels = a_channels;
    self_allocated = true;
  }
}

void Image::scale(int x_scale, int y_scale) {
    Pixel* buf = new Pixel[Width() * Height() * x_scale * y_scale];
    std::cout << Width() * Height() * x_scale * y_scale << '\n';
    for (int i = 0; i < Height(); ++i) {
        for (int j = 0; j < Width(); ++j) {
            for (int loc_i = 0; loc_i < y_scale; ++loc_i) {
                for (int loc_j = 0; loc_j < x_scale; ++loc_j) {
                    //std::cout << ((Width() * x_scale * y_scale) * i + (x_scale) * j) + x_scale * loc_i + loc_j << '\n';
                    buf[((Width() * x_scale * y_scale) * i + (x_scale) * j) + x_scale * loc_i + loc_j] = data[Width() * i + j];
                }
            }
        }
    }
    std::cout << Width() * Height() * x_scale * y_scale << '\n';
    std::cout << "AAA\n\n\n\n";
    delete [] data;
    data = new Pixel[Size() * x_scale * y_scale];
    data = buf;
    width *= x_scale;
    height *= y_scale;
    size *= x_scale * y_scale;
}


int Image::Save(const std::string &a_path)
{
  auto extPos = a_path.find_last_of('.');
  if(a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG")
  {
    stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
  }
  else if(a_path.substr(extPos, std::string::npos) == ".jpg" || a_path.substr(extPos, std::string::npos) == ".JPG" ||
          a_path.substr(extPos, std::string::npos) == ".jpeg" || a_path.substr(extPos, std::string::npos) == ".JPEG")
  {
    stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
  }
  else
  {
    std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
    return 1;
  }

  return 0;
}

Image::~Image()
{
  if(self_allocated)
    delete [] data;
  else
  {
    stbi_image_free(data);
  }
}
