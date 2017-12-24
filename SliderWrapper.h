#ifndef SLIDERWRAPPER_H
#define SLIDERWRAPPER_H

#include <QObject>

class QSlider;

class SliderWrapper : public QObject
{
  Q_OBJECT
public:
  SliderWrapper( QSlider* slider );

  double stepSize();
  double value() const;
  void setRange( double min, double max );
  void setSteps( int steps );
  void setStepSize( double stepSize );
  void setValue( double value );
  void updateSteps(int steps);
  void updateStepSize(double stepSize);
  void updateValue( double value );

public slots:
  void updatePosition(int position);
//  void updateValue( int position );

signals:
  void valueChanged( double );
  void stepSizeChanged( double );

protected:
  double bound( double value );
  double distance( double a, double b );
  bool areEqual( double a, double b );
  void addToValue( double increment );

  QSlider* m_slider;
  double m_min;
  double m_max;
  double m_value;
  double m_stepSize;
  int m_position;
};

#endif  // SLIDERWRAPPER_H
