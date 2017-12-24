#ifndef SLIDERWRAPPER_H
#define SLIDERWRAPPER_H

#include <QObject>

class SliderWrapper : public QObject
{
    Q_OBJECT
public:
    explicit SliderWrapper(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SLIDERWRAPPER_H