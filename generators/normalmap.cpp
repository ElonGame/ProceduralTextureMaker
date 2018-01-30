/**
 * Part of the ProceduralTextureMaker project.
 * http://github.com/johanokl/ProceduralTextureMaker
 * Released under GPLv3.
 * Johan Lindqvist (johan.lindqvist@gmail.com)
 */

#include "core/textureimage.h"
#include <math.h>
#include <QColor>
#include <QVector3D>
#include "normalmap.h"

using namespace std;

NormalMapTextureGenerator::NormalMapTextureGenerator()
{
   QStringList directions;
   directions.append("Up");
   directions.append("Down");
   directions.append("Left");
   directions.append("Right");

   TextureGeneratorSetting direction;
   direction.name = "Direction";
   direction.description = "Which way?";
   direction.defaultvalue = QVariant(directions);
   configurables.insert("direction", direction);
}


void NormalMapTextureGenerator::generate(QSize size,
                                         TexturePixel* destimage,
                                         QMap<int, TextureImagePtr> sourceimages,
                                         TextureNodeSettings* settings) const
{
   if (!destimage || !size.isValid()) {
      return;
   }

   QString direction = configurables["direction"].defaultvalue.toStringList().takeFirst();
   if (settings->contains("direction") && !settings->value("direction").toString().isEmpty()) {
      direction = settings->value("direction").toString();
   }
   memset(destimage, 255, size.width() * size.height() * sizeof(TexturePixel));

   if (!sourceimages.contains(0)) {
      return;
   }
   TexturePixel* sourceImage = sourceimages.value(0)->getData();

   for (int y = 1; y < size.height() - 1; y++) {
      for (int x = 1; x < size.width() - 1; x++) {
         int pixelpos = y * size.width() + x;
         double topleft = sourceImage[(y - 1) * size.width() + x - 1].intensity();
         double topmiddle = sourceImage[(y - 1) * size.width() + x].intensity();
         double topright = sourceImage[(y - 1) * size.width() + x + 1].intensity();
         double middleleft = sourceImage[y * size.width() + x - 1].intensity();
         double middleright = sourceImage[y * size.width() + x + 1].intensity();
         double lowerleft = sourceImage[(y + 1) * size.width() + x - 1].intensity();
         double lowermiddle = sourceImage[(y + 1) * size.width() + x].intensity();
         double lowerright = sourceImage[(y + 1) * size.width() + x + 1].intensity();

         double pStrength = 2;

         const double dX = (topright + 2.0 * middleright + lowerright) - (topleft + 2.0 * middleleft + lowerleft);
         const double dY = (lowerleft + 2.0 * lowermiddle + lowerright) - (topleft + 2.0 * topmiddle + topright);
         const double dZ = 1.0 / pStrength;

         QVector3D vect(dX, dY, dZ);
         vect.normalize();

         destimage[pixelpos].r = (vect.x() + 1.0) * 127.5;
         destimage[pixelpos].g = (vect.y() + 1.0) * 127.5;
         destimage[pixelpos].b = (vect.z() + 1.0) * 127.5;
      }
   }
}
