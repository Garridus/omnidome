#include <slim/RangedInt.h>

#include <QPainter>
#include <QStylePainter>
#include <QStyleOption>
#include <QMouseEvent>
#include <QWheelEvent>

using namespace slim;

RangedInt::RangedInt(QWidget* _parent) :
  AbstractInputWidget(_parent)
{
  init();
}

RangedInt::RangedInt(QString const& _label,
  int _value, int _min, int _max, QWidget* _parent) :
  AbstractInputWidget(_parent),
  mixin_range_type(_value,_min,_max)
{
  setLabel(_label);
  init();
}

RangedInt::~RangedInt()
{
}
 
void RangedInt::setValue(int _value)
{
  mixin_range_type::setValue(_value);
}

void RangedInt::setMinimum(int _min)
{
  mixin_range_type::setMinimum(_min);
}

void RangedInt::setMaximum(int _max)
{
  mixin_range_type::setMaximum(_max);
}


void RangedInt::setRange(int _min, int _max)
{
  mixin_range_type::setRange(_min,_max);
}
    
void RangedInt::valueChangedEvent()
{
  mixin_range_type::apply(editorAs<editor_type>());
  AbstractInputWidget::valueLabel_->setText(QString("%1").arg(value()));
  update();
  emit valueChanged();
}

void RangedInt::rangeChangedEvent()
{
  mixin_range_type::apply(editorAs<editor_type>());
  emit rangeChanged();
}

int RangedInt::valueFromPos(double _pos) const
{
  auto&& _rect = rect();
  return (_pos - _rect.left()) / _rect.width() * range() + minimum();
}

int RangedInt::valueToPos() const
{
  auto&& _rect = rect();
  return _rect.left() + ratio()* float(_rect.width());
}

void RangedInt::paintEvent(QPaintEvent* _paintEvent)
{
  QStylePainter _p(this);
  _p.setRenderHint(QPainter::Antialiasing);
  QStyleOption _option;
  _option.initFrom(this);
  
  double _pos = valueToPos();

  AbstractInputWidget::drawTrack(_p,_pos);
  AbstractInputWidget::drawHandle(_p,_pos);
  AbstractInputWidget::drawBorder(_p,hasFocus() || editor_->hasFocus());
}

void RangedInt::mouseMoveEvent(QMouseEvent* e)
{
  if (moving_)
  {
    setValue(valueFromPos(e->pos().x()));
  }
}

void RangedInt::mousePressEvent(QMouseEvent* e)
{
  AbstractInputWidget::mousePressEvent(e);
  if (abs(e->pos().x() - valueToPos()) <= 4)
  {
    moving_ = true;
  }
}

void RangedInt::mouseReleaseEvent(QMouseEvent* e)
{
  moving_ = false;
}

void RangedInt::wheelEvent(QWheelEvent* e)
{
  if (hasFocus())
    setValue(value() + e->delta()/15.0 / 8.0 * singleStep());
}
  
void RangedInt::setEditorVisible(bool _visible)
{
  AbstractInputWidget::setEditorVisible(_visible);
  mixin_range_type::apply(editorAs<editor_type>());
}

void RangedInt::init()
{
  moving_= false;
  setSingleStep(1.0);

  auto _editor = AbstractInputWidget::createEditor<editor_type>();
 
  connect(_editor,SIGNAL(valueChanged(int)),this,SLOT(setValue(int)));
  _editor->setFrame(false);
  _editor->setAlignment(Qt::AlignRight);
  _editor->setButtonSymbols(QAbstractSpinBox::PlusMinus);
  mixin_range_type::apply(_editor);
  valueChangedEvent();
  hideEditor();
}
