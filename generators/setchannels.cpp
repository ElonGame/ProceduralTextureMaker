/**
 * Part of the ProceduralTextureMaker project.
 * http://github.com/johanokl/ProceduralTextureMaker
 * Released under GPLv3.
 * Johan Lindqvist (johan.lindqvist@gmail.com)
 */

#include <math.h>
#include <QColor>
#include <QtMath>
#include "core/textureimage.h"
#include "setchannels.h"

using namespace std;

SetChannelsTextureGenerator::SetChannelsTextureGenerator()
{
   QStringList channels;
   channels.append("None");
   channels.append("Fill");
   channels.append("First's red");
   channels.append("First's green");
   channels.append("First's blue");
   channels.append("First's alpha");
   channels.append("Second's red");
   channels.append("Second's green");
   channels.append("Second's blue");
   channels.append("Second's alpha");

   TextureGeneratorSetting channelRed;
   channelRed.name = "Source for red";
   channelRed.order = 0;
   channelRed.defaultindex = 2;
   channelRed.defaultvalue = QVariant(channels);
   configurables.insert("channelRed", channelRed);
   TextureGeneratorSetting channelGreen;
   channelGreen.name = "Source for green";
   channelGreen.order = 1;
   channelGreen.defaultindex = 3;
   channelGreen.defaultvalue = QVariant(channels);
   configurables.insert("channelGreen", channelGreen);
   TextureGeneratorSetting channelBlue;
   channelBlue.name = "Source for blue";
   channelBlue.order = 2;
   channelBlue.defaultindex = 4;
   channelBlue.defaultvalue = QVariant(channels);
   configurables.insert("channelBlue", channelBlue);
   TextureGeneratorSetting channelAlpha;
   channelAlpha.name = "Source for alpha";
   channelAlpha.order = 3;
   channelAlpha.defaultindex = 5;
   channelAlpha.defaultvalue = QVariant(channels);
   configurables.insert("channelAlpha", channelAlpha);
}


unsigned char SetChannelsTextureGenerator::getColorFromChannel(TexturePixel &firstColor, TexturePixel &secondColor,
                                                               SetChannelsTextureGenerator::Channels channel) const
{
   switch (channel) {
   case Channels::none:
      return 0;
   case Channels::fill:
      return 255;
   case Channels::node1red:
      return firstColor.r;
   case Channels::node1green:
      return firstColor.g;
   case Channels::node1blue:
      return firstColor.b;
   case Channels::node1alpha:
      return firstColor.a;
   case Channels::node2red:
      return secondColor.r;
   case Channels::node2green:
      return secondColor.g;
   case Channels::node2blue:
      return secondColor.b;
   case Channels::node2alpha:
      return secondColor.a;
   }
   return 0;
}


SetChannelsTextureGenerator::Channels SetChannelsTextureGenerator::getChannelFromName(QString name) const
{
   if (name == "Fill") {
      return Channels::fill;
   }
   if (name == "First's red") {
      return Channels::node1red;
   }
   if (name == "First's green") {
      return Channels::node1green;
   }
   if (name == "First's blue") {
      return Channels::node1blue;
   }
   if (name == "First's alpha") {
      return Channels::node1alpha;
   }
   if (name == "Second's red") {
      return Channels::node2red;
   }
   if (name == "Second's green") {
      return Channels::node2green;
   }
   if (name == "Second's blue") {
      return Channels::node2blue;
   }
   if (name == "Second's alpha") {
      return Channels::node2alpha;
   }
   return Channels::none;
}


void SetChannelsTextureGenerator::generate(QSize size, TexturePixel* destimage,
                                           QMap<int, TextureImagePtr> sourceimages,
                                           TextureNodeSettings* settings) const
{
   if (!destimage || !size.isValid()) {
      return;
   }
   QString channelRedStr = configurables["channelRed"].defaultvalue.toStringList().at(configurables["channelRed"].defaultindex);
   if (settings->contains("channelRed") && !settings->value("channelRed").toString().isEmpty()) {
      channelRedStr = settings->value("channelRed").toString();
   }
   QString channelGreenStr = configurables["channelGreen"].defaultvalue.toStringList().at(configurables["channelGreen"].defaultindex);
   if (settings->contains("channelGreen") && !settings->value("channelGreen").toString().isEmpty()) {
      channelGreenStr = settings->value("channelGreen").toString();
   }
   QString channelBlueStr = configurables["channelBlue"].defaultvalue.toStringList().at(configurables["channelBlue"].defaultindex);
   if (settings->contains("channelBlue") && !settings->value("channelBlue").toString().isEmpty()) {
      channelBlueStr = settings->value("channelBlue").toString();
   }
   QString channelAlphaStr = configurables["channelAlpha"].defaultvalue.toStringList().at(configurables["channelAlpha"].defaultindex);
   if (settings->contains("channelAlpha") && !settings->value("channelAlpha").toString().isEmpty()) {
      channelAlphaStr = settings->value("channelAlpha").toString();
   }

   Channels channelRed = getChannelFromName(channelRedStr);
   Channels channelGreen = getChannelFromName(channelGreenStr);
   Channels channelBlue = getChannelFromName(channelBlueStr);
   Channels channelAlpha = getChannelFromName(channelAlphaStr);

   int numPixels = size.width() * size.height();
   TexturePixel* firstSource = NULL;
   TexturePixel* secondSource = NULL;

   if (sourceimages.contains(0)) {
      firstSource = sourceimages.value(0).data()->getData();
   }
   if (sourceimages.contains(1)) {
      secondSource = sourceimages.value(1).data()->getData();
   }
   if (!firstSource && !secondSource) {
      memset(destimage, 0, numPixels * sizeof(TexturePixel));
      return;
   }
   if (!firstSource) {
      firstSource = new TexturePixel[numPixels];
      memset(firstSource, 0, numPixels * sizeof(TexturePixel));
   } else if (!secondSource) {
      secondSource = new TexturePixel[numPixels];
      memset(secondSource, 0, numPixels * sizeof(TexturePixel));
   }
   for (int thisPos = 0; thisPos < numPixels; thisPos++) {
      destimage[thisPos].r = getColorFromChannel(firstSource[thisPos], secondSource[thisPos], channelRed);
      destimage[thisPos].g = getColorFromChannel(firstSource[thisPos], secondSource[thisPos], channelGreen);
      destimage[thisPos].b = getColorFromChannel(firstSource[thisPos], secondSource[thisPos], channelBlue);
      destimage[thisPos].a = getColorFromChannel(firstSource[thisPos], secondSource[thisPos], channelAlpha);
   }
}