#include "Widget.h"
#include "ui_Widget.h"

#include <QObject>
#include <iostream>
#include "SliderWrapper.h"

#define LINEAR 0
#define LOG 1

//------------------------------------------------------------------------------

// los casos de uso a considerar son:
// - cuando se mueve el slider( o se apreta +/-)
// - cuando se cambia el delta
// - cuando se cambia el ev.

Widget::Widget( QWidget* parent ) : QWidget( parent ), ui( new Ui::Widget )
{
  ui->setupUi( this );

  // set edits to allow double values only
  auto doubleValidator = new QDoubleValidator( this );
  ui->m_minEdit->setValidator( doubleValidator );
  ui->m_maxEdit->setValidator( doubleValidator );
  ui->m_deltaEdit->setValidator( doubleValidator );

  // set a validator for evEdit.
  auto evValidator = new QDoubleValidator( this );
  evValidator->setNotation( QDoubleValidator::ScientificNotation );
  ui->m_valueEdit->setValidator( evValidator );

  QIntValidator* intValidator = new QIntValidator( this );
  ui->m_initStepEdit->setValidator( intValidator );

  m_sliderWrapper = new SliderWrapper( ui->m_valueSlider );

  createConnections();
}

//------------------------------------------------------------------------------

Widget::~Widget()
{
  delete ui;
}

//------------------------------------------------------------------------------

void Widget::createConnections()
{
  // edit controls (field def group)
  connect( ui->m_minEdit, SIGNAL( editingFinished() ), this,
           SLOT( activateValueGroup() ) );
  connect( ui->m_maxEdit, SIGNAL( editingFinished() ), this,
           SLOT( activateValueGroup() ) );
  connect( ui->m_initStepEdit, SIGNAL( editingFinished() ), this,
           SLOT( activateValueGroup() ) );
  connect( ui->m_scaleCombo, SIGNAL( currentIndexChanged( int ) ), this,
           SLOT( activateValueGroup() ) );

  // edit controls (ev group)
  connect( ui->m_valueEdit, SIGNAL( editingFinished() ),
           this, SLOT( setNewValue() ) );
  connect( ui->m_deltaEdit, SIGNAL( editingFinished() ),
           this, SLOT(setNewStepSize()));

  // button controls
  //  connect(ui->m_minusButton, SIGNAL(clicked()), this, SLOT(minusPressed()));
  //  connect(ui->m_plusButton, SIGNAL(clicked()), this, SLOT(plusPressed()));

  // SliderWrapper
  connect( m_sliderWrapper, SIGNAL( valueChanged( double ) ),
           this, SLOT( updateValueEdit( double ) ) );
  connect( m_sliderWrapper, SIGNAL( stepSizeChanged( double ) ),
           this, SLOT( updateStepSizeEdit( double ) ) );
}

//------------------------------------------------------------------------------

void Widget::updateStepSizeEdit( double value )
{
  ui->m_deltaEdit->setText( QString::number( value ) );
}

//------------------------------------------------------------------------------

void Widget::setNewValue()
{
  m_sliderWrapper->updateValue( toDouble( ui->m_valueEdit ) );
}

//------------------------------------------------------------------------------

void Widget::setNewStepSize()
{
  m_sliderWrapper->setStepSize( toDouble(ui->m_deltaEdit) );
}

//------------------------------------------------------------------------------

// valida y activa el slider si se define correctamente el field.
void Widget::activateValueGroup()
{
  if( ui->m_initStepEdit->text().isEmpty() ||
      ui->m_minEdit->text().isEmpty() ||
      ui->m_maxEdit->text().isEmpty() ||
      toDouble( ui->m_minEdit ) >= toDouble( ui->m_maxEdit ) ||
      ui->m_scaleCombo->currentIndex() < 0 )
  {
    ui->m_valueGroup->setEnabled( false );
    return;
  }

  prepareValueGroup();
}

//------------------------------------------------------------------------------

// this is called only when all field info is available and valid.
void Widget::prepareValueGroup()
{
  ui->m_valueGroup->setEnabled( true );
  m_sliderWrapper->setRange( toDouble( ui->m_minEdit ),
                             toDouble( ui->m_maxEdit ) );
  m_sliderWrapper->updateSteps( toInt( ui->m_initStepEdit ) );

  //  ui->m_deltaEdit->setText(QString::number(m_sliderWrapper->stepSize()));
  //    calculateDelta(steps);
  //    calculateSliderMarks(toDouble(ui->m_minEdit));

  // move slider to center
  //    ui->m_valueSlider->setValue(steps/2);
}

//------------------------------------------------------------------------------

void Widget::updateExtractionValue()
{
  double value = toDouble( ui->m_valueEdit );

  m_sliderWrapper->setValue( value );
  //    // if value out of range, recover last.
  //    if( value < toDouble(ui->m_minEdit) || value > toDouble(ui->m_maxEdit))
  //    {
  //        ui->m_valueEdit->setText(QString::number(m_eValue));
  //        return;
  //    }

  //    m_eValue = value;
  //    ui->m_valueSlider->setValue(int(m_eValue/m_delta));
}

//------------------------------------------------------------------------------

void Widget::deltaChanged()
{
}

//------------------------------------------------------------------------------

void Widget::minusPressed()
{
  ui->m_valueSlider->setValue( ui->m_valueSlider->value() - 1 );
}

//------------------------------------------------------------------------------

void Widget::plusPressed()
{
  ui->m_valueSlider->setValue( ui->m_valueSlider->value() + 1 );
}

//------------------------------------------------------------------------------

void Widget::updateValueEdit( double value )
{
  ui->m_valueEdit->setText( QString::number( value ) );
}

//------------------------------------------------------------------------------

//int Widget::calculateSliderMarks( double start )
//{
//  double min = toDouble( ui->m_minEdit );
//  double max = toDouble( ui->m_maxEdit );
//  if( start < min || start > max )
//  {
//    return -1;
//  }
//  int startPos = 0;

//  m_sliderMarks.clear();
//  // fill left side
//  for( double mark = start; mark > min; mark -= m_delta )
//  {
//    m_sliderMarks.push_front( mark );
//    startPos++;
//  }
//  m_sliderMarks.push_front( min );

//  // fill right side
//  for( double mark = start + m_delta; mark < max; mark += m_delta )
//  {
//    m_sliderMarks.push_back( mark );
//  }
//  m_sliderMarks.push_back( max );
//  std::cout << "size marks=" << m_sliderMarks.size() << std::endl;
//  return startPos;
//}

//------------------------------------------------------------------------------

void Widget::calculateDelta( int steps )
{
  if( ui->m_scaleCombo->currentIndex() == LINEAR )
  {
    m_delta =
        ( toDouble( ui->m_maxEdit ) - toDouble( ui->m_minEdit ) ) / ( steps );
  }
  if( ui->m_scaleCombo->currentIndex() == LOG )
  {
    // THIS IS WRONG.
    m_delta =
        ( toDouble( ui->m_maxEdit ) - toDouble( ui->m_minEdit ) ) / ( steps );
  }

  ui->m_deltaEdit->setText( QString::number( m_delta ) );
}

//------------------------------------------------------------------------------

// value and delta are keept as double members to avoid precision issues
// void Widget::setDelta(double v)
//{
//    ui->m_deltaEdit->setText(QString::number(v));
//    m_delta = v;
//}

void Widget::setEValue( double v )
{
  ui->m_valueEdit->setText( QString::number( v ) );
  m_eValue = v;
}

//------------------------------------------------------------------------------

// helpers
int Widget::checkSliderSteps( int steps )
{
  if( steps % 2 > 0 ) steps++;
  return steps;
}

//------------------------------------------------------------------------------

double Widget::toDouble( QLineEdit* le )
{
  return le->text().toDouble();
}

//------------------------------------------------------------------------------

int Widget::toInt( QLineEdit* le )
{
  return le->text().toInt();
}
