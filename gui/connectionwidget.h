/**
 * Part of the ProceduralTextureMaker project.
 * http://github.com/johanokl/ProceduralTextureMaker
 * Released under GPLv3.
 * Johan Lindqvist (johan.lindqvist@gmail.com)
 */

#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QWidget>

class TextureProject;
class ItemInfoPanel;
class QLabel;
class QVBoxLayout;
class QGroupBox;
class QGridLayout;

/**
 * @brief The ConnectionWidget class
 *
 * Simple widget that displays a connections. Currently just lists
 * the names of the source and receiver nodes with a button for
 * disconnecting them.
 *
 * @todo Display the nodes' images similar to PreviewImagePanel
 */
class ConnectionWidget : public QWidget
{
   Q_OBJECT

public:
   explicit ConnectionWidget(ItemInfoPanel* widgetmanager);
   ~ConnectionWidget() override = default;
   void setNodes(int sourceNodeId, int receiverNodeId, int slot);

public slots:
   void disconnectNodes();

private:
   ItemInfoPanel* widgetmanager;
   int sourceNodeId;
   int receiverNodeId;
   int slot;

   QGroupBox* nodeInfoWidget;
   QGridLayout* nodeInfoLayout;
   QLabel* nodeSourceLabel;
   QLabel* nodeReceiverLabel;
   QLabel* nodeSlotLabel;
   QVBoxLayout* layout;
};

#endif // CONNECTIONWIDGET_H
