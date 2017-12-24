#ifndef WIDGET_H
#define WIDGET_H

#include <QList>
#include <QWidget>

class QLineEdit;
class SliderWrapper;

namespace Ui
{
class Widget;
}

class Widget : public QWidget
{
  Q_OBJECT

 public:
  explicit Widget( QWidget* parent = 0 );
  ~Widget();

 public slots:
  void setNewValue();
  void setNewStepSize();
  void updateValueEdit( double value );
  void updateStepSizeEdit( double value );

 protected:
  void createConnections();
  void prepareValueGroup();
  double toDouble( QLineEdit* le );
  int checkSliderSteps( int steps );
  void calculateDelta( int steps );
  void setEValue( double v );
  int toInt( QLineEdit* le );
  void deltaChanged();
//  void calculateSliderMarks( int steps );
//  int calculateSliderMarks( double start );
 protected slots:
  void activateValueGroup();
  void minusPressed();
  void plusPressed();
  void updateExtractionValue();

 private:
  Ui::Widget* ui;
  double m_delta;
  double m_eValue;
  QList<double> m_sliderMarks;
  SliderWrapper* m_sliderWrapper;
};

#endif  // WIDGET_H
