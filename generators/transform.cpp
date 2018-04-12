/**
 * Part of the ProceduralTextureMaker project.
 * http://github.com/johanokl/ProceduralTextureMaker
 * Released under GPLv3.
 * Johan Lindqvist (johan.lindqvist@gmail.com)
 */

#include "transform.h"
#include <QPainter>

TransformTextureGenerator::TransformTextureGenerator()
{
   TextureGeneratorSetting scaleX;
   scaleX.name = "Scale X (%)";
   scaleX.description = "";
   scaleX.defaultvalue = QVariant((double) 100);
   scaleX.min = QVariant(0);
   scaleX.max = QVariant(500);
   scaleX.order = 1;
   scaleX.group = "scale";
   configurables.insert("xscale", scaleX);

   TextureGeneratorSetting scaleY;
   scaleY.name = "Scale Y (%)";
   scaleY.description = "";
   scaleY.defaultvalue = QVariant((double) 100);
   scaleY.min = QVariant(0);
   scaleY.max = QVariant(500);
   scaleY.order = 2;
   scaleY.group = "scale";
   configurables.insert("yscale", scaleY);

   TextureGeneratorSetting rotation;
   rotation.name = "Rotation";
   rotation.description = "";
   rotation.defaultvalue = QVariant((double) 0);
   rotation.min = QVariant(-360);
   rotation.max = QVariant(360);
   rotation.order = 3;
   configurables.insert("rotation", rotation);

   TextureGeneratorSetting offsetLeft;
   offsetLeft.name = "Offset left";
   offsetLeft.description = "";
   offsetLeft.defaultvalue = QVariant((double) 0);
   offsetLeft.min = QVariant(-200);
   offsetLeft.max = QVariant(200);
   offsetLeft.order = 4;
   configurables.insert("offsetleft", offsetLeft);

   TextureGeneratorSetting offsetTop;
   offsetTop.name = "Offset top";
   offsetTop.description = "";
   offsetTop.defaultvalue = QVariant((double) 0);
   offsetTop.min = QVariant(-200);
   offsetTop.max = QVariant(200);
   offsetTop.order = 5;
   configurables.insert("offsettop", offsetTop);

   TextureGeneratorSetting colorsetting;
   colorsetting.name = "Background";
   colorsetting.defaultvalue = QVariant(QColor(0, 0, 0, 0));
   colorsetting.order = 6;
   configurables.insert("backgroundcolor", colorsetting);

   TextureGeneratorSetting firstXtiles;
   firstXtiles.name = "Tiles 1st pass (X)";
   firstXtiles.description = "";
   firstXtiles.defaultvalue = QVariant((int) 1);
   firstXtiles.min = QVariant(1);
   firstXtiles.max = QVariant(5);
   firstXtiles.group = "first tiles";
   firstXtiles.order = 7;
   configurables.insert("firstXtiles", firstXtiles);

   TextureGeneratorSetting firstYtiles;
   firstYtiles.name = "Tiles 1st pass (Y)";
   firstYtiles.description = "";
   firstYtiles.defaultvalue = QVariant((int) 1);
   firstYtiles.min = QVariant(1);
   firstYtiles.max = QVariant(5);
   firstYtiles.group = "first tiles";
   firstYtiles.order = 8;
   configurables.insert("firstYtiles", firstYtiles);

   TextureGeneratorSetting secondXtiles;
   secondXtiles.name = "Tiles 2nd pass (X)";
   secondXtiles.description = "";
   secondXtiles.defaultvalue = QVariant((int) 1);
   secondXtiles.min = QVariant(1);
   secondXtiles.max = QVariant(50);
   secondXtiles.group = "second tiles";
   secondXtiles.order = 9;
   configurables.insert("secondXtiles", secondXtiles);

   TextureGeneratorSetting secondYtiles;
   secondYtiles.name = "Tiles 2nd pass (Y)";
   secondYtiles.description = "";
   secondYtiles.defaultvalue = QVariant((int) 1);
   secondYtiles.min = QVariant(1);
   secondYtiles.max = QVariant(50);
   secondYtiles.group = "second tiles";
   secondYtiles.order = 10;
   configurables.insert("secondYtiles", secondYtiles);
}


void TransformTextureGenerator::generate(QSize size,
                                         TexturePixel* destimage,
                                         QMap<int, TextureImagePtr> sourceimages,
                                         TextureNodeSettings* settings) const
{
   if (!settings || !destimage || !size.isValid()) {
      return;
   }

   double scaleX = settings->value("xscale").toDouble() / 100;
   double scaleY = settings->value("yscale").toDouble() / 100;
   double rotation = settings->value("rotation").toDouble();
   int offsetLeft = settings->value("offsetleft").toDouble() * size.width() / 100;
   int offsetTop = settings->value("offsettop").toDouble() * size.height() / 100;
   int firstXtiles = settings->value("firstXtiles").toInt();
   int firstYtiles = settings->value("firstYtiles").toInt();
   int secondXtiles = settings->value("secondXtiles").toInt();
   int secondYtiles = settings->value("secondYtiles").toInt();
   QColor backgroundcolor = settings->value("backgroundcolor").value<QColor>();

   QImage tempimage(size.width(), size.height(), QImage::Format_ARGB32);

   if (sourceimages.contains(0)) {
      memcpy(tempimage.bits(), sourceimages.value(0)->getData(), size.width() * size.height() * sizeof(TexturePixel));
   } else {
      memset(tempimage.bits(), 0, size.width() * size.height() * sizeof(TexturePixel));
   }

   QImage tiledimage(size.width(), size.height(), QImage::Format_ARGB32);
   memset(tiledimage.bits(), 0, size.width() * size.height() * sizeof(TexturePixel));

   QPainter firstpainter(&tiledimage);
   firstpainter.scale((double) 1 / secondXtiles, (double) 1 / secondYtiles);
   firstpainter.fillRect(0, 0, secondXtiles * tempimage.width(), secondYtiles * tempimage.height(), QBrush(tempimage));

   QImage destBuffer(firstXtiles * size.width(), firstYtiles * size.height(), QImage::Format_ARGB32);
   TexturePixel col(backgroundcolor.red(), backgroundcolor.green(), backgroundcolor.blue(), backgroundcolor.alpha());

   auto* destBufferPixels = (TexturePixel*) destBuffer.bits();
   int numPixels = firstXtiles * size.width() * firstYtiles * size.height();
   for (int i=0; i < numPixels; i++) {
      *destBufferPixels = col;
      destBufferPixels++;
   }

   QPainter painter(&destBuffer);
   painter.translate(offsetLeft, offsetTop);
   painter.translate(size.width() / 2, size.height() / 2);
   painter.rotate(rotation);
   painter.scale(scaleX, scaleY);
   painter.translate(-firstXtiles * size.width() / 2, -firstYtiles * size.height() / 2);
   painter.fillRect(0, 0, destBuffer.width(), destBuffer.height(), QBrush(tiledimage));

   memset(tempimage.bits(), 0, size.width() * size.height() * sizeof(TexturePixel));

   QPainter finalpainter(&tempimage);
   finalpainter.drawImage(QRectF(0, 0, size.width(), size.height()),
                          destBuffer,
                          QRectF(0, 0, size.width(), size.height()));

   memcpy(destimage, tempimage.bits(), size.width() * size.height() * sizeof(TexturePixel));
}
