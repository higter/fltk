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
  min_value_->input_type(FL_FLOAT_INPUT);
  min_value_->value(minValue_);
  min_value_->callback((Fl_Callback*)cb_min_value, this);
  max_value_ = new Fl_Input(0,0,0,0);
  max_value_->input_type(FL_FLOAT_INPUT);
  max_value_->value(maxValue_);
  max_value_->callback((Fl_Callback*)cb_max_value, this);
}


void Fl_Value_Range_Slider::on_min_value_changed()
{
  minmax_value(min_value_->dvalue(), maxValue_);
  min_value_->value(minValue_);
  value_damage();
  set_changed();
}

void Fl_Value_Range_Slider::on_max_value_changed()
{
  minmax_value(minValue_, max_value_->dvalue());
  max_value_->value(maxValue_);
  value_damage();
  set_changed();
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
  {
      fl_push_clip(sxx+Fl::box_dx(box()),
                  syy+Fl::box_dy(box()),
                  sww-Fl::box_dw(box()),
                  shh-Fl::box_dh(box()));
      draw_box();
      fl_pop_clip();
  }
  // if (damage() & FL_DAMAGE_ALL)
  {
    if (horizontal())
    {
      draw_box(box(), sxx, syy+shh/2-2, sww, 4, color());
    }
    else
    {
      draw_box(box(), sxx+sww/2-2, syy, 4, shh, color());
    }
  }
  
  draw_slider(sxx+Fl::box_dx(box()),
                  syy+Fl::box_dy(box()),
                  sww-Fl::box_dw(box()),
                  shh-Fl::box_dh(box()));


  min_value_->resize(bxx,byy,bww,bhh);
  max_value_->resize(exx,eyy,eww,ehh);
}

void Fl_Value_Range_Slider::draw_slider(int X, int Y, int W, int H)
{
    double minVal = value_to_position(min_value());
    double maxVal = value_to_position(max_value());
    double delVal = maxVal - minVal;
  
    int ww = (horizontal() ? W : H);
    int xx, S;
    {
      S = int(delVal*ww+.5);
      // if(minimum()>maximum()) 
      // {
      //   S = ww-S;
      //   xx = ww-S;
      // }
      // else
      {
        xx = int(minVal*ww+.5);
      } 
    } 
    int xsl, ysl, wsl, hsl;
    if(horizontal())
    {
      xsl = X+xx;
      wsl = S;
      ysl = Y;
      hsl = H;
    } 
    else 
    {
      ysl = Y+xx;
      hsl = S;
      xsl = X;
      wsl = W;
    }

  // draw_bg(X, Y, W, H, S);
    // {
    //     fl_push_clip(X, Y, W, H);
    //     draw_box();
    //     fl_pop_clip();
    // }

    // Fl_Boxtype box1 = slider();
    // if (!box1) 
    // {
    //   box1 = (Fl_Boxtype)(box()&-2); 
    //   if(!box1)
    //   box1 = FL_UP_BOX;
    // }
    {
      if (wsl>0 && hsl>0) 
      {
        static int ss = 8;//(slider_size() * ww + 0.5f);
        draw_box(FL_UP_BOX, xsl, ysl+hsl/2-ss/2, wsl, hsl-ss, selection_color());
        static int slw = 4;
        if(horizontal())
        {
          draw_box(FL_UP_BOX, xsl-slw/2, ysl, slw, hsl, FL_GRAY);
          draw_box(FL_UP_BOX, xsl+wsl-slw/2, ysl, slw, hsl, FL_GRAY);
        }
        else
        {
          draw_box(FL_UP_BOX, xsl, ysl-slw/2, wsl, slw, FL_GRAY);
          draw_box(FL_UP_BOX, xsl, ysl+hsl-slw/2, wsl, slw, FL_GRAY);    
        }
      }

    //   if (Fl::is_scheme("gtk+")) 
    //   {
    //       if (W>H && wsl>(hsl+8))
    //       {
    //           // Draw horizontal grippers
    //           int yy, hh;
    //           hh = hsl-8;
    //           xx = xsl+(wsl-hsl-4)/2;
    //           yy = ysl+3;

    //           fl_color(fl_darker(selection_color()));
    //           fl_line(xx, yy+hh, xx+hh, yy);
    //           fl_line(xx+6, yy+hh, xx+hh+6, yy);
    //           fl_line(xx+12, yy+hh, xx+hh+12, yy);

    //           xx++;
    //           fl_color(fl_lighter(selection_color()));
    //           fl_line(xx, yy+hh, xx+hh, yy);
    //           fl_line(xx+6, yy+hh, xx+hh+6, yy);
    //           fl_line(xx+12, yy+hh, xx+hh+12, yy);
    //       } 
    //       else if(H>W && hsl>(wsl+8))
    //       {
    //         // Draw vertical grippers
    //         int yy;
    //         xx = xsl+4;
    //         ww = wsl-8;
    //         yy = ysl+(hsl-wsl-4)/2;

    //         fl_color(fl_darker(selection_color()));
    //         fl_line(xx, yy+ww, xx+ww, yy);
    //         fl_line(xx, yy+ww+6, xx+ww, yy+6);
    //         fl_line(xx, yy+ww+12, xx+ww, yy+12);

    //         yy++;
    //         fl_color(fl_lighter(selection_color()));
    //         fl_line(xx, yy+ww, xx+ww, yy);
    //         fl_line(xx, yy+ww+6, xx+ww, yy+6);
    //         fl_line(xx, yy+ww+12, xx+ww, yy+12);
    //       }
    //   }
    }

    draw_label(xsl, ysl, wsl, hsl);
    // if (Fl::focus() == this) 
    // {
    //     draw_focus();
    // }
}

int Fl_Value_Range_Slider::handle(int event, int X, int Y, int W, int H)
{
  static int pushType = 0;
  switch (event) {
  case FL_PUSH: {
    Fl_Widget_Tracker wp(this);
    if (!Fl::event_inside(X, Y, W, H)) return 0;
    handle_push();
    {
        int ww = (horizontal() ? W : H);
        int mx = (horizontal() ? Fl::event_x()-X : Fl::event_y()-Y);
        int minXX = int(value_to_position(min_value()) * ww + 0.5f);
        int maxXX = int(value_to_position(max_value()) * ww + 0.5f);
        pushType = 0;
        if(mx < minXX-2)
        {
          pushType = 1;
        }
        else if(mx < minXX+2)
        {
            pushType = 2;
        }
        else if(mx < maxXX-2)
        {
            pushType = 3;
        }
        else if(mx < maxXX+2)
        {
            pushType = 4;
        }
        else if(mx < ww)
        {
            pushType = 5;
        }
    }
    if (wp.deleted()) return 1; }
    // fall through ...
  case FL_DRAG: {

    double val;
    if (minimum() == maximum())
      val = 0.5;
    else
      val = value_to_position(min_value());

      
    int ww = (horizontal() ? W : H);
    int mx = (horizontal() ? Fl::event_x()-X : Fl::event_y()-Y);
    int minXX = int(value_to_position(min_value()) * ww + 0.5f);
    int maxXX = int(value_to_position(max_value()) * ww + 0.5f);
    int S;
    static int offcenter;

    // if (type() == FL_HOR_FILL_SLIDER || type() == FL_VERT_FILL_SLIDER) 
    {

      S = 0;
      if (event == FL_PUSH) {
        int xx = int(val*ww+.5);
        offcenter = mx-xx;
        if (offcenter < -10 || offcenter > 10) offcenter = 0;
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
    // drag_move(clamp(v));
    } return 1;
  case FL_RELEASE:
    handle_release();
    {
      // int ww = (horizontal() ? W : H);
      // int mx = (horizontal() ? Fl::event_x()-X : Fl::event_y()-Y);
      // double pp = mx / double(ww);
      // double v = position_to_value(pp);
      // if(pushType == 1)
      // {
      //   min_value(v);
      // }
      // else if(pushType == 5)
      // {
      //   max_value(v);
      // }
    }
    return 1;
  case FL_KEYBOARD:
    { Fl_Widget_Tracker wp(this);
      switch (Fl::event_key()) {
        case FL_Up:
          if (horizontal()) return 0;
          handle_push();
          if (wp.deleted()) return 1;
          drag_move((increment_lin_log(0, -1, H)));
          if (wp.deleted()) return 1;
          handle_release();
          return 1;
        case FL_Down:
          if (horizontal()) return 0;
          handle_push();
          if (wp.deleted()) return 1;
          drag_move((increment_lin_log(0, 1, H)));
          if (wp.deleted()) return 1;
          handle_release();
          return 1;
        case FL_Left:
          if (!horizontal()) return 0;
          handle_push();
          if (wp.deleted()) return 1;
          drag_move((increment_lin_log(0, -1, W)));
          if (wp.deleted()) return 1;
          handle_release();
          return 1;
        case FL_Right:
          if (!horizontal()) return 0;
          handle_push();
          if (wp.deleted()) return 1;
          drag_move((increment_lin_log(0, 1, W)));
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
    sww -= 2*value_width();
  } else {
    syy += value_height();
    shh -= 2*value_height();
  }
  return handle(event, sxx + Fl::box_dx(box()), syy + Fl::box_dy(box()),
                           sww - Fl::box_dw(box()), shh - Fl::box_dh(box()));
}

void Fl_Value_Range_Slider::drag_move(double v)
{
    double delVal = maxValue_ - minValue_;
    minValue_+=v;
    maxValue_+=v;
    if(minValue_ < minimum())
    {
      minValue_ = minimum();
      maxValue_ = minValue_+delVal;
    }

    if(maxValue_ > maximum())
    {
      maxValue_ = maximum();
      minValue_ = maxValue_ - delVal;
    }

    min_value_->value(minValue_);
    max_value_->value(maxValue_);

    value_damage();
    set_changed();
}


// Fl_Hor_Value_Slider::Fl_Hor_Value_Slider(int X,int Y,int W,int H,const char *l)
// : Fl_Value_Range_Slider(X,Y,W,H,l) {
//   type(FL_HOR_SLIDER);
// }
