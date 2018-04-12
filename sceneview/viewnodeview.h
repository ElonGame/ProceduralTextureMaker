/**
 * Part of the ProceduralTextureMaker project.
 * http://github.com/johanokl/ProceduralTextureMaker
 * Released under GPLv3.
 * Johan Lindqvist (johan.lindqvist@gmail.com)
 */

#ifndef VIEWNODEVIEW_H
#define VIEWNODEVIEW_H

#include <QGraphicsView>
#include <QObject>

/**
 * @brief The ViewNodeView class
 *
 * Displays a ViewNodeScene instance with ViewNodeItem/ViewNodeLine objects.
 * Supports zooming and scrolling, both with mouse dragging and scrollbars.
 */
class ViewNodeView : public QGraphicsView
{
public:
   explicit ViewNodeView(int defaultZoom = 100);
   ~ViewNodeView() override = default;

public slots:
   void resetZoom();
   void setDefaultZoom(int zoom);

protected:
   void wheelEvent(QWheelEvent* event) override;

private:
   double scrollZoomFactor;
   double defaultZoomFactor;
};

#endif // VIEWNODEVIEW_H
