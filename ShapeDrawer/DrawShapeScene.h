#ifndef QGraphicsScenes_
#define QGraphicsScenes_

#include <QGraphicsScene>

class QGScene : public QGraphicsScene
{
    Q_OBJECT

public:
    QGScene(QObject *parent = nullptr);

public slots:
    void UpDateZoom(qreal val);
protected:
};

#endif // QGraphicsScenes_

