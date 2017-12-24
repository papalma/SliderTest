#include "SliderWrapper.h"

#include <QSlider>
#include <cmath>
#include <iostream>

const double EPSILON = 1.0e-7;
// const int INIT_NO_STEP = 100;

//------------------------------------------------------------------------------
// TODO: falta definir claramente como es que se mueve el slider y se actualizan
// los valores en el wraper
// - moviendo el slider -> valueChanged() -> actualizar valores -> emitir para
// mostrar en pantalla
// - ingrensando valor ->

SliderWrapper::SliderWrapper( QSlider* slider )
    : m_slider( slider ),
      m_min( 0.0 ),
      m_max( 0.0 ),
      m_value( 0.0 ),
      m_stepSize( 0.0 ),
      m_position( 0 )
{
  m_slider->setMinimum( 0 );
  m_slider->setMaximum( 0 );
  m_slider->setValue( 0 );
  connect( m_slider, SIGNAL( sliderMoved( int ) ), this,
           SLOT( updatePosition( int ) ) );
}

//------------------------------------------------------------------------------

void SliderWrapper::setRange( double min, double max )
{
  if( areEqual( min, max ) )
  {
    return;
  }
  if( min > max )
  {
    qSwap( min, max );
  }
  m_min = min;
  m_max = max;
  updatePosition( m_slider->value() );
}

//------------------------------------------------------------------------------

void SliderWrapper::addToValue( double increment )
{
  setValue( bound( m_value + increment ) );
}

//------------------------------------------------------------------------------

void SliderWrapper::updatePosition( int position )
{
//  std::cout << __FUNCTION__ << "()" << " position=" << position << std::endl;
//  if( m_position + 1 == position )
//  {
//    addToValue( m_stepSize );
//  }
//  else if( m_position - 1 == position )
//  {
//    addToValue( -m_stepSize );
//  }
//  else
//  {
    setValue( m_min + position * m_stepSize );
//  }
  m_position = position;
  emit valueChanged( m_value );
}

//------------------------------------------------------------------------------

double SliderWrapper::value() const
{
  return m_value;
}

//------------------------------------------------------------------------------

void SliderWrapper::setValue( double value )
{
  m_value = bound( value );
  m_slider->setValue( int( ceil( distance( m_min, value ) / m_stepSize )) );
}

//------------------------------------------------------------------------------

void SliderWrapper::updateValue( double value )
{
  setValue( value );
  emit valueChanged( m_value );
  //  value = bound( value );
  //  if( areEqual( m_min, value ) )
  //  {
  //    m_slider->setValue( 0 );
  //    setValue( value );
  //    return;
  //  }
  //  if( areEqual( m_max, value ) )
  //  {
  //    m_slider->setValue( m_slider->maximum() );
  //    setValue( value );
  //    return;
  //  }
  //  m_slider->setValue( int( distance( m_min, value ) / m_stepSize ) );
  //  setValue( value );
}

//------------------------------------------------------------------------------

double SliderWrapper::bound( double value )
{
  if( value < m_min )
  {
    return m_min;
  }
  if( value > m_max )
  {
    return m_max;
  }
  return value;
}

//------------------------------------------------------------------------------

double SliderWrapper::distance( double a, double b )
{
  return qAbs( a - b );
}

//------------------------------------------------------------------------------

bool SliderWrapper::areEqual( double a, double b )
{
  return distance( a, b ) < EPSILON;
}

//------------------------------------------------------------------------------

void SliderWrapper::setSteps( int steps )
{
  if( areEqual( m_min, m_max ) )
  {
    return;
  }
  m_stepSize = distance( m_min, m_max ) / ( steps - 1 );
  m_slider->setMaximum( steps - 1 );
}

//------------------------------------------------------------------------------

void SliderWrapper::updateSteps( int steps )
{
  setSteps( steps );
  emit stepSizeChanged( m_stepSize );
}

//------------------------------------------------------------------------------

double SliderWrapper::stepSize()
{
  return m_stepSize;
}

//------------------------------------------------------------------------------

void SliderWrapper::setStepSize( double stepSize )
{
  if( areEqual( stepSize, 0.0 ) )
  {
    return;
  }
  m_slider->setMaximum( int( ceil( distance( m_min, m_max ) / stepSize ) ) );
  m_stepSize = stepSize;
}

//------------------------------------------------------------------------------

void SliderWrapper::updateStepSize( double stepSize )
{
  setStepSize( stepSize );
  emit stepSizeChanged( m_stepSize );
}
