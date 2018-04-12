/**
 * Part of the ProceduralTextureMaker project.
 * http://github.com/johanokl/ProceduralTextureMaker
 * Released under GPLv3.
 * Johan Lindqvist (johan.lindqvist@gmail.com)
 */

#include "boxblur.h"

BoxBlurTextureGenerator::BoxBlurTextureGenerator()
{
   TextureGeneratorSetting neighbourssetting;
   neighbourssetting.defaultvalue = QVariant((int) 5);
   neighbourssetting.name = "Blur level";
   neighbourssetting.min = QVariant(0);
   neighbourssetting.max = QVariant(30);
   configurables.insert("numneighbours", neighbourssetting);
}


void BoxBlurTextureGenerator::generate(QSize size,
                                       TexturePixel* destimage,
                                       QMap<int, TextureImagePtr> sourceimages,
                                       TextureNodeSettings* settings) const
{
   if (!settings || !destimage || !size.isValid()) {
      return;
   }
   if (!sourceimages.contains(0)) {
      memset(destimage, 0, size.width() * size.height() * sizeof(TexturePixel));
      return;
   }
   TexturePixel* sourceImage = sourceimages.value(0).data()->getData();
   if (settings->value("numneighbours").toInt() == 0) {
      memcpy(destimage, sourceImage, size.width() * size.height() * sizeof(TexturePixel));
      return;
   }
   int numNeightboursX = settings->value("numneighbours").toDouble() * qMax(size.width() / 250, 1);
   int numNeightboursY = settings->value("numneighbours").toDouble() * qMax(size.height() / 250, 1);
   for (int j = 0; j < size.height(); j++) {
      for (int i = 0; i < size.width(); i++) {
         int startX = i - numNeightboursX;
         int endX = i + numNeightboursX;
         int startY = j - numNeightboursY;
         int endY = j + numNeightboursY;
         int totalPixels = 0;
         quint64 red = 0;
         quint64 green = 0;
         quint64 blue = 0;
         quint64 alpha = 0;
         for (int ypos = startY; ypos < endY; ypos++) {
            int currY = ypos;
            if (currY < 0) {
               currY += size.height() * 5;
            }
            currY %= size.height();
            currY *= size.width();
            for (int xpos = startX; xpos < endX; xpos++) {
               int currX = xpos;
               if (currX < 0) {
                  currX += size.width() * 5;
               }
               currX %= size.width();
               totalPixels++;
               TexturePixel sourcePixel = sourceImage[currY + currX];
               red += sourcePixel.r;
               green += sourcePixel.g;
               blue += sourcePixel.b;
               alpha += sourcePixel.a;
            }
         }
         int pixelpos = j * size.width() + i;
         destimage[pixelpos].r = static_cast<unsigned char>(red / totalPixels);
         destimage[pixelpos].g = static_cast<unsigned char>(green / totalPixels);
         destimage[pixelpos].b = static_cast<unsigned char>(blue / totalPixels);
         destimage[pixelpos].a = static_cast<unsigned char>(alpha / totalPixels);
      }
   }
}
