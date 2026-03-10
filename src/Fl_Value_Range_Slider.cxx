//
// Value Slider widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2026 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//

#include <FL/Fl.H>
#include <FL/Fl_Value_Range_Slider.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/fl_draw.H>
#include <math.h>


void cb_min_value(Fl_Widget* o, void* data)
{
  Fl_Value_Range_Slider* widget = (Fl_Value_Range_Slider*)data;
  widget->on_min_value_changed();
}

void cb_max_value(Fl_Widget* o, void* data)
{
  Fl_Value_Range_Slider* widget = (Fl_Value_Range_Slider*)data;
  widget->on_max_value_changed();
}
/**
  Creates a new Fl_Value_Range_Slider widget using the given
  position, size, and label string. The default boxtype is FL_DOWN_BOX.
*/
Fl_Value_Range_Slider::Fl_Value_Range_Slider(int X, int Y, int W, int H, const char*l)
: Fl_Slider(X,Y,W,H,l) {
  step(1,100);
  textfont_ = FL_HELVETICA;
  textsize_ = 10;
  textcolor_ = FL_FOREGROUND_COLOR;
  value_width_ = 35;
  value_height_ = 25;
  minValue_ = minimum();
  maxValue_ = maximum();


  min_value_ = new Fl_Input(0,0,0,0);
  min_value_->value(minValue_);
  min_value_->callback((Fl_Callback*)cb_min_value, this);
  max_value_ = new Fl_Input(0,0,0,0);
  max_value_->value(maxValue_);
  // max_value_->callback((Fl_Callback*)cb_max_value, this);
}


void Fl_Value_Range_Slider::on_min_value_changed()
{
  minValue_ = min_value_->dvalue();
  value(minValue_);
}

void Fl_Value_Range_Slider::on_max_value_changed()
{
  maxValue_ = max_value_->dvalue();
}

void Fl_Value_Range_Slider::draw() {
  int sxx = x(), syy = y(), sww = w(), shh = h();
  int bxx = x(), byy = y(), bww = w(), bhh = h();
  int exx = x(), eyy = y(), eww = w(), ehh = h();
  if (horizontal()) {
    bww = value_width();
    exx = x() + w() - value_width();
    eww = value_width();
    sxx += value_width();
    sww -= 2 * value_width();
  } else {
    syy += value_height();
    exx = x() + h() - value_height();
    eww = value_height();    
    bhh = value_height();
    shh -= 2 * value_height();
  }
  if (damage() & FL_DAMAGE_ALL)
    draw_box(box(), sxx, syy, sww, shh, color());
  Fl_Slider::draw(sxx+Fl::box_dx(box()),
                  syy+Fl::box_dy(box()),
                  sww-Fl::box_dw(box()),
                  shh-Fl::box_dh(box()));
  min_value_->resize(bxx,byy,bww,bhh);
  // min_value_->value(value());

  max_value_->resize(exx,eyy,eww,ehh);
  // max_value_->value(value());
  // draw_box(box(),bxx,byy,bww,bhh,color());
  // std::string buf = format_str();
  // fl_font(textfont(), textsize());
  // fl_color(active_r() ? textcolor() : fl_inactive(textcolor()));
  // fl_draw(buf.c_str(), bxx, byy, bww, bhh, FL_ALIGN_CLIP);
}

int Fl_Value_Range_Slider::handle(int event, int X, int Y, int W, int H)
{
  switch (event) {
  case FL_PUSH: {
    Fl_Widget_Tracker wp(this);
    if (!Fl::event_inside(X, Y, W, H)) return 0;
    handle_push();
    if (wp.deleted()) return 1; }
    // fall through ...
  case FL_DRAG: {

    double val;
    if (minimum() == maximum())
      val = 0.5;
    else
      val = value_to_position(value());

    int ww = (horizontal() ? W : H);
    int mx = (horizontal() ? Fl::event_x()-X : Fl::event_y()-Y);
    int S;
    static int offcenter;

    if (type() == FL_HOR_FILL_SLIDER || type() == FL_VERT_FILL_SLIDER) {

      S = 0;
      if (event == FL_PUSH) {
        int xx = int(val*ww+.5);
        offcenter = mx-xx;
        if (offcenter < -10 || offcenter > 10) offcenter = 0;
        else return 1;
      }

    } else {

      S = int(slider_size()*ww+.5); if (S >= ww) return 0;
      int T = (horizontal() ? H : W)/2+1;
      if (type()==FL_VERT_NICE_SLIDER || type()==FL_HOR_NICE_SLIDER) T += 4;
      if (S < T) S = T;
      if (event == FL_PUSH) {
        int xx = int(val*(ww-S)+.5);
        offcenter = mx-xx;
        if (offcenter < 0) offcenter = 0;
        else if (offcenter > S) offcenter = S;
        else return 1;
      }
    }

    int xx = mx-offcenter;
    double v = 0;
    char tryAgain = 1;
    while (tryAgain)
    {
      tryAgain = 0;
      if (xx < 0) {
        xx = 0;
        offcenter = mx; if (offcenter < 0) offcenter = 0;
      } else if (xx > (ww-S)) {
        xx = ww-S;
        offcenter = mx-xx; if (offcenter > S) offcenter = S;
      }
      // Convert position back to value using the appropriate scale
      double pos = (ww-S) > 0 ? double(xx) / double(ww-S) : 0.0;
      v = round(position_to_value(pos));
      // make sure a click outside the sliderbar moves it:
      if (event == FL_PUSH && v == value()) {
        offcenter = S/2;
        event = FL_DRAG;
        tryAgain = 1;
      }
    }
    handle_drag(clamp(v));
    } return 1;
  case FL_RELEASE:
    handle_release();
    return 1;
  case FL_KEYBOARD:
    { Fl_Widget_Tracker wp(this);
      switch (Fl::event_key()) {
        case FL_Up:
          if (horizontal()) return 0;
          handle_push();
          if (wp.deleted()) return 1;
          handle_drag(clamp(increment_lin_log(value(), -1, H)));
          if (wp.deleted()) return 1;
          handle_release();
          return 1;
        case FL_Down:
          if (horizontal()) return 0;
          handle_push();
          if (wp.deleted()) return 1;
          handle_drag(clamp(increment_lin_log(value(), 1, H)));
          if (wp.deleted()) return 1;
          handle_release();
          return 1;
        case FL_Left:
          if (!horizontal()) return 0;
          handle_push();
          if (wp.deleted()) return 1;
          handle_drag(clamp(increment_lin_log(value(), -1, W)));
          if (wp.deleted()) return 1;
          handle_release();
          return 1;
        case FL_Right:
          if (!horizontal()) return 0;
          handle_push();
          if (wp.deleted()) return 1;
          handle_drag(clamp(increment_lin_log(value(), 1, W)));
          if (wp.deleted()) return 1;
          handle_release();
          return 1;
        default:
          return 0;
      }
    }
    // break not required because of switch...
  case FL_FOCUS :
  case FL_UNFOCUS :
    if (Fl::visible_focus()) {
      redraw();
      return 1;
    } else return 0;
  case FL_ENTER :
  case FL_LEAVE :
    return 1;
  default:
    return 0;
  }
}

int Fl_Value_Range_Slider::handle(int event) {
  if (event == FL_PUSH && Fl::visible_focus()) {
    Fl::focus(this);
    redraw();
  }
  int sxx = x(), syy = y(), sww = w(), shh = h();
  if (horizontal()) {
    sxx += value_width();
    sww -= value_width();
  } else {
    syy += value_height();
    shh -= value_height();
  }
  return handle(event, sxx + Fl::box_dx(box()), syy + Fl::box_dy(box()),
                           sww - Fl::box_dw(box()), shh - Fl::box_dh(box()));
}


// Fl_Hor_Value_Slider::Fl_Hor_Value_Slider(int X,int Y,int W,int H,const char *l)
// : Fl_Value_Range_Slider(X,Y,W,H,l) {
//   type(FL_HOR_SLIDER);
// }
