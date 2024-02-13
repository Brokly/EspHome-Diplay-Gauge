#pragma once

#include "esphome/components/display/display.h"
#include "esphome/components/output/float_output.h"

namespace esphome {
namespace display_gauge {

static const char *const TAG = "DisplayGauge";

#define DEF_HEIGHT 16 // дефолтная толщина градусника
#define DEF_WIDTH  60 // дефолтная ширина прямого градусника
#define DEF_RADIUS 50 // дефолтный радиус спидометра
#define DEF_ANGLE  0  // дефолтный угол поворота прямого градусника
#define DEF_RAD_ANGLE_START 340 // дефолтный угол начала спидометра
#define DEF_RAD_ANGLE_END 200 // дефолтный угол конца спидометра

class DisplayGauge : public output::FloatOutput, public Component {
 public:
  
  //DisplayGauge() {}
  
  // ПАРAМЕТРЫ
  // дисплей на котором рисуем
  void set_display(display::Display *display) { this->display_ = display;}
  // установка координат градусника ( центр расположения)
  void set_x(int32_t x){this->xc_=x;} 
  void set_y(int32_t y){this->yc_=y;} 
  // ширина градусника
  void set_width(uint32_t width){this->width_=width; this->circle_=false;} 
  // высота градусника
  void set_height(uint32_t height){this->height_=height;} 
  // высота градусника
  void set_radius(uint32_t radius){this->radius_=radius; this->circle_=true;} 
  // размер ползунка
  void set_knob(uint16_t size){this->knob_=size;}
  void set_knob_width(uint16_t size){this->knob_width_=size; this->knob_circle_=false;}
    // стиль ползунка
  void set_knob_soft(bool val){this->knob_soft_=val; this->knob_circle_=false;}
  // углы раскрытия градусника
  void set_ang0(uint16_t ang){this->ang0_=ang;} // стартовый угол 
  void set_ang100(uint16_t ang){this->ang100_=ang; this->circle_=true;} // конечный угол
  // минимальный и максимальный цвета  
  void set_color0(Color cl){  // стартовый цвет
      this->col0_=cl; 
      this->col100_=cl;
      this->col50_=cl;
  }
  void set_color100(Color cl){ // конечный цвет
      this->col100_=cl; 
  }
  void set_color50(Color cl){ // конечный цвет
      this->col50_=cl; 
      this->colset50_=true;
  }
  void set_border_color(Color cl){ // цвет бордюра
     this->colbord_=cl; // цвет бордюра
     this->borderAlterColor_=true; // флаг бордюра альтернативного цвета
  }
  // установка цвета задника
  void set_bkg_color(Color cl){ // цвет фона
      this->bkgcolor_=cl;
      this->bkgAlterColor_=true; // флаг наличия цвета задника
  }
  // альтернативный цвет заполнения прогресса
  void set_colorStart(Color cl){ // цвет заполнения при наличии ползунка
      this->startcolor_=cl;
      startAlterColor_=true;// флаг активации альтернативного цвета заполнения
  }
  // цвет для 3d бордюра
  void set_3d_color(Color cl){ 
      this->color3d_=cl;
      alter3dColor_=true;// флаг активации эффекта 3D
  }
  // установка стиля
  void set_soft(bool val){this->soft_=val;}
  // признак автообновленя
  void set_repaint(bool val){this->auto_repaint_=val;}
  // признак обратного отсчета (против часовой стрелки)
  void set_mirroring(bool val){this->mirror_=val;}
  
  void setup() override {
      if(this->ang0_>=360) { // установка дефолтных параметров зависящих от типа градусника
         if(this->circle_){
           this->ang0_=DEF_RAD_ANGLE_START;
         } else {             
           this->ang0_=DEF_ANGLE;
         }
      }
      if(!this->circle_){
         // рассчет координат по углу
         get_rect_angle(this->height_, this->width_, 360+180-this->ang0_, &(this->x0_), &(this->y0_),
                       &(this->x1_), &(this->y1_), &(this->x2_), &(this->y2_), &(this->x3_), &(this->y3_));
      } else { // вращение стандартных координат к рассчетным, для спидометров
         this->ang0_ = ((360+270-this->ang0_)%360);
         this->ang100_ = ((360+270-this->ang100_)%360);
      }
      if(this->knob_==0){
         if(this->knob_width_!=0){
            this->knob_=knob_width_;
         } else if (this->knob_soft_){ // если закругленный
            this->knob_width_=this->height_;
            this->knob_=this->height_;
         } 
      }
  }
  
  void loop() override {};

  void dump_config() override{
     ESP_LOGCONFIG(TAG, "DISPLAY GAUGE:");
     ESP_LOGCONFIG(TAG, "  Center coordintes X:%d, Y:%d", this->xc_, this->yc_);
     if(this->circle_){
        ESP_LOGCONFIG(TAG, "  Circle style:");
        ESP_LOGCONFIG(TAG, "     Width:%u, Radius:%u", this->height_, this->radius_);
        ESP_LOGCONFIG(TAG, "     Angles start:%u, end:%u", (90+this->ang0_)%360,(90+this->ang100_)%360);
     } else {
        ESP_LOGCONFIG(TAG, "  Line style:");
        ESP_LOGCONFIG(TAG, "     Height:%u, Width:%u, Till angle:%d", this->height_, this->width_, (180+this->ang0_)%360);
     }
     if(this->knob_!=0){
        if(knob_circle_){
           ESP_LOGCONFIG(TAG, "  Witch circle knob:");
           ESP_LOGCONFIG(TAG, "     Knob radius:%d", this->knob_);
        } else {
           ESP_LOGCONFIG(TAG, "  Witch rectngle knob:");
           ESP_LOGCONFIG(TAG, "     Knob width:%d, height:d", this->knob_width_, this->knob_);
           ESP_LOGCONFIG(TAG, "     Soft style:", YESNO(this->soft_));
        }
     }
     if(this->borderAlterColor_){
        ESP_LOGCONFIG(TAG, "  Border color:0x%X", this->colbord_.raw_32);
        if(this->alter3dColor_){
           ESP_LOGCONFIG(TAG, "  Border 3D color:0x%X", this->color3d_.raw_32);
        }
     }
     if(this->bkgAlterColor_){
        ESP_LOGCONFIG(TAG, "  Background color:0x%X", this->bkgcolor_.raw_32);
     }
     if(this->col0_==this->col100_){
        ESP_LOGCONFIG(TAG, "  Mono color:0x%X", this->col0_.raw_32);
     } else if(this->colset50_){
        ESP_LOGCONFIG(TAG, "  Triple colors - start:0x%X, half::0x%X, end:0x%X", this->col0_.raw_32, this->col50_.raw_32, this->col100_.raw_32);
     } else {        
        ESP_LOGCONFIG(TAG, "  Duo colors - start:0x%X, end:0x%X", this->col0_.raw_32, this->col100_.raw_32);
     }
     if(this->startAlterColor_){
        ESP_LOGCONFIG(TAG, "  Fill color:0x%X", this->startcolor_.raw_32);
     }
     ESP_LOGCONFIG(TAG, "  Soft style:", YESNO(this->soft_));
     ESP_LOGCONFIG(TAG, "  Auto repaint:", YESNO(this->auto_repaint_));
     ESP_LOGCONFIG(TAG, "  Mirroring:", YESNO(this->mirror_));
  } 

  float get_setup_priority() const override { return esphome::setup_priority::DATA;}
  
  // рисование градусника
  void repaint(){
    if(this->display_ != nullptr){
        // РАССЧЕТ ЦВЕТА
        Color col = this->col0_;
        float invval= 1.0-val_;
        if(this->colset50_ && (this->col0_!=this->col50_ || this->col50_!=this->col100_)){
           col=color_gauge(this->col100_, this->col50_, this->col0_, invval); //определяем цвет для линейки из трех
        } else if(this->col0_ != this->col100_){
           col=color_gauge(this->col100_, this->col0_, invval); //определяем цвет печати для линейки из двух
        }
        Color knobcol=col;
        if(this->knob_!=0){ // если есть движок, то цвет меняется только у него
           if(this->startAlterColor_){
              col=this->startcolor_;
           } else {
              col=this->col0_;
           }
        }
        // РИСОВАНИЕ ЗАОЛНЕНИЕ И БАКГРАУНД
        int32_t dx;
        int32_t dy;
        int32_t ang;
        if(this->circle_){
           // стандартный способ развертки дуги по часовой стрелке 50->150=260гр , 150->50=100гр
           // окаймляющая дуга всегда рисуется стандартным способом развертки
           // для стандартного режима:
           // начало дуги заполнения всегда должно лежать в начале окаймляющей дуги (ang_0)
           // конец дуги заполнения должен быть между началом и концом, таких вариантов два
           // длинна окаймляющей дуги в градусах рассчитывается для разных случаев по разному
           int32_t tHeight=this->height_; 
           if(this->borderAlterColor_){
              tHeight -=2;//заливка меньше ширины на 2 пикселя
           }
           if(this->mirror_){ // обратный (зеркальный, против часовой стрелки) градусник
              if(this->ang100_ > this->ang0_){ //вычисление угла конечной точки
                 ang=ang0_-round((invval)*(360.0-ang100_+ang0_))+360;      
              } else {
                 ang=round((this->val_*(ang0_-ang100_))+ang100_);      
              }
              //ang %=360;
              if(this->bkgAlterColor_){ // установлено фоновое заполнение
                 arc(this->xc_, this->yc_, this->radius_, ang, this->ang0_, this->bkgcolor_, tHeight, this->soft_, true); // рисуем заполнение для зеркалированного спидометра
              }
              arc(this->xc_, this->yc_, this->radius_, this->ang100_, ang, col, tHeight, this->soft_, true); // рисуем заполнение для зеркалированного спидометра
           } else {
              if(this->ang100_ > this->ang0_){ //вычисление угла конечной точки
                 ang=ang0_-round(this->val_*(360-ang100_+ang0_))+360;      
              } else {
                 ang=round(invval*(ang0_-ang100_))+ang100_;      
              }
              //ang %=360;
              if(this->bkgAlterColor_){ // установлено фоновое заполнение
                 arc(this->xc_, this->yc_, this->radius_, this->ang100_, ang, this->bkgcolor_, tHeight, this->soft_, true); // рисуем заполнение для зеркалированного спидометра
              }
              arc(this->xc_, this->yc_, this->radius_, ang, this->ang0_, col, tHeight, this->soft_, true); // рисуем заполнение спидометра
           }
        } else { // градусник в виде полоски 
           dx=round(this->val_*(this->x2_-this->x1_));
           dy=round(this->val_*(this->y2_-this->y1_));
           //dx=(this->val_*(this->x2_-this->x1_));
           //dy=(this->val_*(this->y2_-this->y1_));
           if(this->mirror_){
              if(this->bkgAlterColor_){ // фоновое заполнение, при необходимости
                 rect(this->xc_, this->yc_, this->x0_, this->y0_, this->x1_, this->y1_, 
                      this->x2_-dx, this->y2_-dy, this->x3_-dx, this->y3_-dy, 
                      this->height_, this->ang0_, this->bkgcolor_, this->soft_, true);
              }
              // заполнение линии показаний
              rect(this->xc_, this->yc_, this->x2_, this->y2_, this->x3_, this->y3_, 
                   this->x3_-dx, this->y3_-dy, this->x2_-dx, this->y2_-dy, 
                   this->height_, this->ang0_+180, col, this->soft_, true);
           } else {
              if(this->bkgAlterColor_){ // фоновое заполнение, при необходимости
                 rect(this->xc_, this->yc_, this->x2_, this->y2_, this->x3_, this->y3_, 
                      this->x0_+dx, this->y0_+dy, this->x1_+dx, this->y1_+dy, 
                      this->height_, this->ang0_+180, this->bkgcolor_, this->soft_, true);
              }
              rect(this->xc_, this->yc_, this->x0_, this->y0_, this->x1_, this->y1_, 
                   this->x1_+dx, this->y1_+dy, this->x0_+dx, this->y0_+dy, 
                   this->height_, this->ang0_, col, this->soft_, true);
           }
        }
        
        // РИСУЕМ БОРДЮР
        Color colbord=col;
        if(this->borderAlterColor_){ // бордюр альтернативного цвета
           colbord=this->colbord_;
           if(circle_){ // бордюр круглого градусника
              if(this->alter3dColor_){ // дополнительный 3D бордюр
                 arc(this->xc_-1, this->yc_-1, this->radius_, this->ang100_, 
                    this->ang0_, this->color3d_, this->height_, this->soft_); // потом рисуем контур
              }
              arc(this->xc_, this->yc_, this->radius_, this->ang100_, 
                  this->ang0_, colbord, this->height_, this->soft_); // потом рисуем контур
           } else { // бордюр прямого градусника
              if(this->alter3dColor_){ // дополнительный 3D бордюр
                 rect(this->xc_, this->yc_, this->x0_, this->y0_, this->x1_, this->y1_, this->x2_, this->y2_, 
                      this->x3_, this->y3_, this->height_, this->ang0_, this->color3d_, this->soft_);
              }
              rect(this->xc_-1, this->yc_-1, this->x0_, this->y0_, this->x1_, this->y1_, this->x2_, this->y2_, 
                   this->x3_, this->y3_, this->height_, this->ang0_, colbord, this->soft_);
           }
        }

        //ang %=360;
        // РИСУЕМ УКАЗАТЕЛЬ
        if(this->knob_!=0){ // в стиле , с указателем
           if(this->circle_){ // указатель на кривом градуснике
              coord(this->radius_, ang, &dx, &dy); //вычисление координат указателя для спидометра
              ESP_LOGE("","%u",ang);
              ang=360+90-ang;
           } else { //указатель на прямом градуснике
              if(this->mirror_){
                 dx=((this->x3_+this->x2_)/2)-dx;
                 dy=((this->y3_+this->y2_)/2)-dy;
              } else {
                 dx=((this->x1_+this->x0_)/2)+dx;
                 dy=((this->y1_+this->y0_)/2)+dy;
              }
              ang=360+90-this->ang0_;
           }
           //ang %=360;
           dx+=this->xc_;
           dy+=this->yc_;
           if(this->knob_circle_){ // круглый ползунок
              this->display_->filled_circle(dx, dy, this->knob_, knobcol); // рисуем окружностями
              if(this->borderAlterColor_){ // если нужен бордюр
                 if(this->alter3dColor_){ //  если нужен дополнительный 3D бордюр
                    bres_arc(dx-1, dy-1, this->knob_, 0, 359, this->color3d_);
                 }                    
                 bres_arc(dx, dy, this->knob_, 0, 359, this->colbord_);
              }
           } else { // прямоугольный ползунок
              int32_t x0; int32_t y0;
              int32_t x1; int32_t y1;
              int32_t x2; int32_t y2;
              int32_t x3; int32_t y3;
              get_rect_angle(this->knob_width_, this->knob_ , ang , &x0, &y0, &x1, &y1, &x2, &y2, &x3, &y3);// пересчет координат с учетом угла поворота
              ang=(360+180-ang)%360; //пересчет угла в координаты рисования прямоугольников
              rect(dx, dy, x0, y0, x1, y1, x2, y2, x3, y3, this->knob_width_, ang, knobcol, this->knob_soft_, true); //тело движка
              if(this->borderAlterColor_){ // если нужен бордюр
                 if(this->alter3dColor_){ // если нужен дополнительный 3D бордюр
                    rect(dx-1, dy-1, x0, y0, x1, y1, x2, y2, x3, y3, this->knob_width_, ang, this->color3d_, this->knob_soft_, false); // бордюр 3D
                 }                    
                 rect(dx, dy, x0, y0, x1, y1, x2, y2, x3, y3, this->knob_width_, ang, this->colbord_, this->knob_soft_, false); // бордюр
              }
           }
        }
     }
  }
  
  // установка нового значения градусника
  void write_state(float state) override {
     if(this->val_!=state){
        this->val_=state;
        if(this->auto_repaint_){
           repaint();    
        }
     }
  }
  
 protected:

  display::Display* display_{nullptr};
  int32_t xc_=0; // координаты центра градусника
  int32_t yc_=0;
  int32_t x0_=0; // координаты вершин прямоугольного градусника
  int32_t y0_=0; // рассчитываются в момент загрузки
  int32_t x1_=0;
  int32_t y1_=0;
  int32_t x2_=0;
  int32_t y2_=0;
  int32_t x3_=0;
  int32_t y3_=0;
  uint16_t height_=DEF_HEIGHT; // высота градусника
  uint16_t width_=DEF_WIDTH; // ширина градусника для стандартного вида
  uint16_t radius_=DEF_RADIUS; // радиус градусника
  int16_t ang100_=DEF_RAD_ANGLE_END; // углы раскрытия
  int16_t ang0_=360; 
  float val_=0.0; //значение показаний oт 0 до 1
  Color col0_={255,255,255}; // стартовый цвет
  bool colset50_=false; // признак установки промежуточного
  Color col100_={255,255,255}; // цвет при полном заполнении
  Color col50_={255,255,255}; // цвет при 50% заполнении
  Color colbord_={255,255,255}; // цвет бордюра
  Color bkgcolor_={0,0,0}; // цвет фона
  Color startcolor_={0,0,0}; // цвет заполнения при наличии ползунка
  Color color3d_={0,0,0}; // цвет дополнительного бордюра для эекта 
  bool alter3dColor_=false; // лаг наличия эффекта 3D
  bool startAlterColor_=false; // флаг наличия цвета задника
  bool bkgAlterColor_=false; // флаг наличия цвета задника
  bool borderAlterColor_=false; // флаг бордюра альтернативного цвета
  bool soft_=false; // признак "мягкого" стиля - закругление торцов
  bool auto_repaint_=true; // автоматическая перерисовка при установке значения
  bool circle_=false; // признак круглого градусника - СПИДОМЕТР
  bool mirror_=false; // зеркалировать (спидометр растет против часовой стрелки), плоский разворачивается на 180гр.
  uint16_t knob_=0; // размер указателя
  uint16_t knob_width_=0; // ширина указателя
  bool knob_circle_=true; // признак круглого указателя
  bool knob_soft_=false; // признак стиля ползунка
  
  
    //функция определения осьмушки, к которой принадлежит угол    
    uint8_t octant(int16_t angle){
      return (angle%360)/45;
    }

    //процедура определения координат точки дуги по углу
    void coord(uint16_t radius, int16_t angle, int32_t* x, int32_t* y){
      angle %=360;
      float ang = (PI * angle )/180.0; //преобразование градусов в радианы
      *y=(int32_t)(round(cos(ang)*radius));
      *x=(int32_t)(round(sin(ang)*radius));
    }

    // рассчет радиуса по центру окружности и точке лежащей на окружности
    float getRad(int32_t xc, int32_t yc, int32_t x, int32_t y){
       return sqrt(pow((float)xc - x,2) + pow((float)yc - y,2));
    }
    
    // рассчет угла к точке лежащей на окружности
    int16_t getAngle(int32_t xc, int32_t yc, int32_t x, int32_t y , float r){
       if(r<=0){
          return 0;
       }
       float c = acos(((float)(y-yc))/r);
       float ang = c*180.0/PI;
       if(x-xc<0){
          ang=360.0-ang;
       }
       return round(ang);
    }

    //процедура коррекции координат для осьмушки
    //k - номер осьмушки (от 0 до 7)
    //x, y - координаты точки в 7 осьмушке
    //xout, yout - преобразованные координаты для осьмушки номер k
    inline void HOT sim(int32_t x, int32_t y, uint8_t k, int32_t* xout, int32_t* yout){
       //if     (k==0)  {*xout =  y; *yout =  x;} //0001
       //else if(k==1)  {*xout =  x; *yout =  y;} //0000
       //else if(k==2)  {*xout =  x; *yout = -y;} //0010
       //else if(k==3)  {*xout =  y; *yout = -x;} //0011
       //else if(k==4)  {*xout = -y; *yout = -x;} //0111
       //else if(k==5)  {*xout = -x; *yout = -y;} //0110
       //else if(k==6)  {*xout = -x; *yout =  y;} //0100
       //else /*(k==7)*/{*xout = -y; *yout =  x;} //0101
       
       // Мне кажется , что так быстрее
       const uint8_t core[]={1,0,2,3,7,6,4,5};
       register uint8_t temp=core[k];
       register int32_t tempX;
       register int32_t tempY;
       if(temp&1){
          tempX=y; tempY=x;      
       } else {
          tempX=x; tempY=y;      
       }
       if(temp&2){
          tempY=-tempY;   
       }
       if(temp&4){
          tempX=-tempX;   
       }
       *xout=tempX;
       *yout=tempY;
    }

   //процедура рисования линии
   //xs, ys - начало линии
   //xe, ye - конец линии
   //cl - цвет точек
   void HOT line(int32_t xs, int32_t ys, int32_t xe, int32_t ye, Color cl){
      if(xs!=xe || ys!=ye){ // наклонная линия
         this->display_->line(xs, ys, xe, ye, cl);
      } else if(xs==xe){ // вертикальная линиия
         if(ys>ye){
            this->display_->vertical_line(xs, ye, ys-ye, cl);   
         } else {
            this->display_->vertical_line(xs, ys, ye-ys, cl);   
         }
      } else /*if(ys==ye)*/{ // горизонтальная линия
         if(xs>xe){
            this->display_->horizontal_line(xe, ys, xs-xe, cl);   
         } else {
            this->display_->horizontal_line(xs, ys, xe-xs, cl);   
         }
      }
   }
   
   //процедура рисования точки
   //xc, yc - координаты центра окружности, которой принадлежит дуга
   //x, y - координаты рисования точки дуги относительно центра окружности
   //cl - цвет точек
   //till - признак работы в повернутых координатах
   //sizePoint - размер точки
   //lineCent - рисовать не точку, а линию к базовой точке
   void HOT point(int32_t xc, int32_t yc, int32_t x, int32_t y, Color cl, bool till=false, bool lineCent=false, uint8_t sizePoint=1){
      if(till){
         int32_t temp=x;
         x=y;
         y=-temp;
      }
      if(lineCent){ //рисуем линию к центру
         if(sizePoint<2){
            line(xc, yc, xc+x, yc+y, cl);
         } else if(sizePoint==2){
            line(xc, yc, xc+x, yc+y, cl); // рисуем 4 линии к центральной точке (сегмент)
            line(xc, yc, xc+x+1, yc+y+1, cl);
            line(xc, yc, xc+x, yc+y+1, cl);
            line(xc, yc, xc+x+1, yc+y, cl);
         } else if(sizePoint>2){ // не проработано !!!  TODO: ПОДУМАТЬ НАД РЕАЛИЗАЦИЕЙ
            uint8_t temp=sizePoint/2;
            this->display_->filled_rectangle(xc+x-temp, yc+y-temp, sizePoint, sizePoint, cl); // рисуем квадратиками
            // НУЖНО НАЙТИ КУДА ДЕЛАСЬ filled_triangle !!!!!
            //this->display_->filled_triangle(xc, yc, xc+x+temp, yc+y+temp, xc+x-temp, yc+y-temp, cl);
         }
      } else {
         if(sizePoint<2){ // рисуем точку
            this->display_->draw_pixel_at(xc+x, yc+y, cl); //рисуем точку
         } else if(sizePoint==2){
            this->display_->draw_pixel_at(xc+x, yc+y, cl); //рисуем точку из 4 точек
            this->display_->draw_pixel_at(xc+x+1, yc+y+1, cl); 
            this->display_->draw_pixel_at(xc+x, yc+y+1, cl); 
            this->display_->draw_pixel_at(xc+x+1, yc+y, cl); 
         } else if(sizePoint>2){ // не проработано !!!  TODO: ПОДУМАТЬ НАД РЕАЛИЗАЦИЕЙ
            uint8_t temp=sizePoint/2;
            this->display_->filled_rectangle(xc+x-temp, yc+y-temp, sizePoint, sizePoint, cl); // рисуем квадратиками
            //this->display_->filled_circle(xc+x, yc+y, sizePoint/2, cl); // рисуем окружностями
         }
      }
   }

   // рассчет среднего угла, для печати дуги двумя полудугами
   //a - угол начала дуги в градусах
   //b - угол конца дуги в градусах
   int16_t getMid(int16_t a, int16_t b){
      if(a>b){
         int16_t c=360-a; // угол смещения для нормализации рассчета углов
         c=((b+c)/2)-c; // средний угол
         if(c<0){ // если угол вылез в минус
            c+=360; // вернем его в нормальное состояние
         }
         return c;
      } 
      return (a+b)/2; //средний угол между концами в нормальном случае
    }
   
   //процедура растровой развёртки дуги по алгоритму Брезенхема
   //xc, yc - координаты центра окружности, которой принадлежит дуга
   //r - радиус дуги
   //a - угол начала дуги в градусах
   //b - угол конца дуги в градусах
   //disp - дисплей на котором рисуем
   //color - цвет точек
   //sizePnt - размер пера
   //first - признак необходимости проверки расположения конца и начала
   //#define SPIX // способ рисования линии
   #define DLT 25 // размер неудобного сектора в градусах
   void HOT bres_arc(int32_t xc, int32_t yc, uint16_t r, uint16_t a, uint16_t b, Color color, uint8_t sizePnt=1, bool filled=false, bool first=true){
      bool till=false; 
      a%=360; // нормализвия углов
      b%=360;  
      if(first){
         if((b>90-DLT && b<90+DLT) || (b>270-DLT && b<270+DLT)){ // конечная точка в неудобном секторе
            if(a>360-DLT || a<DLT || (a>180-DLT && a<180+DLT)){ // и начальная точка в неудобном секторе
               int16_t c=getMid(a,b);
               bres_arc(xc, yc, r, a, c, color, sizePnt, filled, false); // первую половину сектора выводим без вращения координат
               a=c; // корректровка начала следующего полусектора, для вывода с поворотом коордиинат
            }
            till=true;
         } else if((a>90-DLT && a<90+DLT) || (a>270-DLT && a<270+DLT)){ // начальная точка в неудобном секторе
            if(b>360-DLT || b<DLT || (b>180-DLT && b<180+DLT)){ // и конечная точка в неудобном секторе
               int16_t c=getMid(a,b);
               bres_arc(xc, yc, r, c, b, color, sizePnt, filled, false); // вторую половину сектора выводим без вращения координат
               b=c; // корректровка конца первого полусектора, для вывода с поворотом коордиинат
            }
            till=true;
         }
         if(till){
            a=(a+270)%360;
            b=(b+270)%360;  
         }
      }
      uint8_t oa = octant(a); //вычисление номера осьмушки с началом дуги
      uint8_t ob = octant(b); //вычисление номера осьмушки с концом дуги
      int32_t xa; int32_t ya;
      coord( r, a, &xa, &ya); //вычисление координат начала дуги
      int32_t xb; int32_t yb;
      coord( r, b, &xb, &yb); //вычисление координат конца дуги
      int32_t x = r; //координаты начальной точки осьмушки, 
      int32_t y = 0; //в которой происходит растровая развёртка
      int32_t xo; int32_t yo;
      int8_t o;
      int32_t d = 1 - 2 * x; //начальное значение отклонение
      uint8_t doub=0; // флаг печати двойных точек
      while(y<=x){//цикл растровой развёртки, выполняется, пока не развернём всю осьмушку)
         if (oa!=ob || a>b){ //если дуга лежит более более чем в одной осьмушке, то 
            o = oa; //рисуем от начала дуги
            sim(x, y, oa, &xo, &yo); //вычисляем координаты точки для осьмушки с началом дуги
            if ((yo<0 && xo<=xa) || (yo>0 && xo>=xa)) { //если точка принадлежит дуге, то
               point(xc, yc, xo, yo, color, till, filled, sizePnt);   
            }
            o = (o+1) % 8; //номер следующей осьмушки
            while (o != ob){ //пока не достигли осьмушки с концом дуги, рисуем осьмушки целиком
               sim(x, y, o, &xo, &yo); //координаты точки для текущей осьмушки
               point(xc, yc, xo, yo, color, till, filled, sizePnt);   
               o = (o+1) % 8; //номер следующей осьмушки
            }
            sim(x, y, ob, &xo, &yo); //координаты точки для осьмушки с концом дуги
            if ((yo<0 && xo>=xb) || (yo>0 && xo<=xb)){ //если точка принадлежит дуге, то 
               point(xc, yc, xo, yo, color, till, filled, sizePnt);   
            }
               
         } else { // если лежит в одном секторе
            sim(x, y, oa, &xo, &yo); //координаты точки
            if (((yo<0 && xo<=xa) || (yo>0 && xo>=xa)) && //если точка принадлежит дуге, то
                ((yo<=0 && xo>=xb) || (yo>=0 && xo<=xb))) {
               point(xc, yc, xo, yo, color, till, filled, sizePnt);   
            }
         }
         #ifdef SPIX
            // этот алгоритм не рисует смежные пиксели
            d += d < 0 ? 4 * y + 6 : 4 * (y - x--) + 10;
            y++;
         #else         
            // этот алгоритм рисует смежные пиксели
            if(doub & 1){
               d += 2 * (y - x--); 
               doub++;
               continue; 
            }
            if(d < 0) { 
               d += 2 * ++y + 1; 
               continue; 
            } 
            if(d > 0) { 
               d += 1 - 2 * --x; 
               continue; 
            } 
            doub++;
            y++;
         #endif
      }
   }      

   //дуга СПИДОМЕТРА без заливки
   //xc, yc - координаты центра окружности, которой принадлежит дуга
   //r - радиус дуги
   //a - угол начала дуги в градусах
   //b - угол конца дуги в градусах
   //color - цвет точек
   //size - ширина фигуры
   //soft - признак закругленных концов дуги
   //fill - признак заполненной дуги
   void HOT arc(int32_t xc, int32_t yc, uint16_t r, uint16_t a, uint16_t b, Color color, uint16_t size=1, bool soft=true, bool fill=false){
      if(size<3){
         // для малой тощины рисуем фигню
         bres_arc(xc, yc, r, a, b, color, size);   
      } else {
         // рисование тела
         uint16_t size2=size/2;
         r-=size2;
         bres_arc(xc, yc, r, a, b, color); // дуга внутреняя
         if(fill){
            for(uint16_t i=1; i<size; i++){
               bres_arc(xc, yc, r+i, a, b, color); // дуги заполнения
            }
         }
         int16_t rb=r+size2*2;
         bres_arc(xc, yc, rb, a, b, color); // дуга внешняя
         
         // рисование торцов
         int32_t xa; int32_t ya;
         int32_t xb; int32_t yb;
         coord(r, a, &xa, &ya);
         coord(++rb, a, &xb, &yb);
         if(soft){  //рисуем закругленый торец начала
            bres_arc(xc+((xa+xb)/2), yc+((ya+yb)/2), size2, a+180, a, color, 1, fill);
         } else { //рисуем торцевую линию начала
            this->display_->line(xb+xc, yb+yc, xa+xc, ya+yc, color); 
         }
         coord(r, b, &xa, &ya);
         coord(rb, b, &xb, &yb);
         if(soft){ //рисуем закругленый торец конца
            bres_arc(xc+((xa+xb)/2), yc+((ya+yb)/2), size2, b, b+180, color, 1, fill);
         } else { //рисуем торцевую линию конца
            this->display_->line(xb+xc, yb+yc, xa+xc, ya+yc, color);
         }
      }
   }    
   
   //процедура рисования прямоуголика по вершинам
   //xc, yc - координаты центра объекта
   //x0, y0 - координаты нижнего левого угла относительно центра объекта
   //x1, y1 - координаты верхнего левого угла относительно центра объекта
   //x2, y2 - координаты верхнего правого угла относительно центра объекта
   //x3, y3 - координаты нижнего правого угла относительно центра объекта
   //height - высота прямоугольника, не обязательно, можно установить 0, тогда поссчитается сам, но дольше по времени
   //ang - угол наклона фигуры, не обязательно, можно установить 360,  тогда поссчитается сам, но дольше по времени  
   //color - цвет точек
   //soft - признак закругления торцов
   //fill - признак заливки
   #define PIX4 // способ рисования линии 4x связные или 8и связные
   void HOT rect(int32_t xc, int32_t yc, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint16_t height, int16_t ang, Color color, bool soft=false, bool fill=false){
      const int32_t dx = abs(x0 - x1); 
      int32_t dy = abs(y0 - y1);
      const int8_t sy = y1 < y0 ? 1 : -1;
      const int8_t sx = x1 < x0 ? 1 : -1;
      const int32_t delx = x2-x1;
      const int32_t dely = y2-y1;
      x1+=xc;
      y1+=yc;
      x0+=xc;
      y0+=yc;
      if(soft){ // закругленные торцы
         int32_t xc=(x0+x1)/2; // координаты центра заглушки
         int32_t yc=(y0+y1)/2;
         if(height==0){
            height=round(getRad(xc, yc, x0, y0)); // радиус заглушки рассчитан по координатам
         } else {
            height/=2; // известна высота фигуры, ее половина и есть радиус
         }            
         if(ang>359){ // угол наклона компонента не известен
            ang=getAngle(xc, yc, x0, y0 , height); //  поссчитаем сложным методом из координат
         } 
         bres_arc(xc, yc, height, ang+180, ang, color, 1, fill); // рисуем полукруглый торец конца с немного умеьшиным радиусом
         bres_arc(xc+delx, yc+dely, height, ang, ang+180, color, 1, fill); // рисуем полукруглый торец начала
      } else if (!fill){ // для пустотелых нужно нарисовать еще прямые торцы
         this->display_->line(x0, y0, x1, y1, color);   
         this->display_->line(xc+x2, yc+y2, xc+x3, yc+y3, color);   
      }
      if(fill){ // заполненный пряммоугольник
       #ifdef PIX4 // c наложением линий
         int32_t err = 0;
         for (int32_t i=0; i < dx+dy; i++) {
            this->display_->line(x1, y1, x1+delx, y1+dely, color);
            int32_t e1 = err + dy;
            int32_t e2 = err - dx;
            if (abs(e1) < abs(e2)) {
               x1 += sx;
               err = e1;
            } else {
               y1 += sy;
               err = e2;
            }
         }
       #else // линия - ступеньками
         dy=-dy;
         int32_t err = dx + dy;
         while (true) {
            this->display_->line(x1, y1, x1+delx, y1+dely, color);
            if (x1 == x0 && y1 == y0) break;
            int32_t e2 = 2 * err;
            if (e2 >= dy) {
               err += dy;
               x1 += sx;
            }
            if (e2 <= dx) {
               err += dx;
               y1 += sy;
            }
         }
       #endif
      } else { // пустотелый прямоугольник
         this->display_->line(x0, y0, xc+x3, yc+y3, color);   
         this->display_->line(xc+x2, yc+y2, x1, y1, color);   
      }
   }

   //рассчет координат прямоугольника по центру, размеру и углу поворота
   //height - высота прямоугольника
   //width - ширина прямоугольника
   //angle - угол поворота
   //x0,y0...x3,y3 - рассчитанные координаты прямоугольника для рисования
   void get_rect_angle(int32_t height, int32_t width, int16_t angle, int32_t* x0, int32_t* y0,
                       int32_t* x1, int32_t* y1,  int32_t* x2, int32_t* y2, int32_t* x3, int32_t* y3){
      angle %=360;
      // рассчет координат по углу
      if(angle==0){
         *x0=*x1= width/2;   //x;
         *x2=*x3= -*x1;      //-x;
         *y2=*y1= height/2;  //y;
         *y0=*y3= -*y1;      //-y;
         return;
      } else if(angle==90){
         *x0=*x3= height/2;  //y;
         *x2=*x1= -*x3;      //-y;
         *y0=*y1= width/2;   //x ;
         *y2=*y3= -*y1;      //-x;
         return;
      } else if(angle==180) {
         *x2=*x3= width/2;   //x;
         *x0=*x1= -*x3;      //-x;
         *y0=*y3= height/2;  //y;
         *y2=*y1= -*y3;      //-y;
         return;
      } else if(angle==270) {
         *x2=*x1= height/2;  //y;
         *x0=*x3= -*x1;      //-y;
         *y2=*y3= width/2;   //x;
         *y0=*y1= -*y3;      //-x;
         return;
      }
      float ang = ((float)PI*angle)/ 180.0; // угол в радианы
      float sin_=sin(ang);             
      //float cos_=pow(1.0-pow(sin_,2),0.5);
      //if(angle>90 and angle<270){
      //   cos_=-cos_;
      //}
      float cos_=cos(ang);
      float y=(float)height/2; 
      float r=(float)width/2; // радиус окружности на которой будут лежать вершины прямоугольника
      #define cos_x ang
      cos_x=cos_*r;
      #define sin_x r
      sin_x=sin_*r;
      #define cos_y cos_
      cos_y=cos_*y;
      #define sin_y sin_
      sin_y=sin_*y;
      // синтез повернутых координат прямоугольного градусника
      *x1= round(cos_x  - sin_y);  //cos_* x   - sin_* y;
      *y1= round(sin_x  + cos_y);  //sin_* x   + cos_* y;
      *x3= round(-cos_x + sin_y);  //cos_*(-x) - sin_*(-y);
      *y3= round(-sin_x - cos_y);  //sin_*(-x) + cos_*(-y);
      *x2= round(-cos_x - sin_y);  //cos_*(-x) — sin_*y; 
      *y2= round(-sin_x + cos_y);  //sin_*(-x) + cos_*y; 
      *x0= round(cos_x  + sin_y);  //cos_* x   — sin_*(-y); 
      *y0= round(sin_x  - cos_y);  //sin_* x   + cos_*(-y); 
   }
   
   // рисование прямоугольника с поворотом на угол (квадратный указатель) c бордюром или без
   // xc, yc - координаты центра прямоугольника
   // width - размер
   // ang - угол поворота
   // col - цвет
   // fill - заполнение
/*
   void rect_knob(int32_t xc, int32_t yc, uint16_t height, uint16_t width,  int16_t ang, Color col, Color colBord, bool soft=false){
      int32_t x0; int32_t y0;
      int32_t x1; int32_t y1;
      int32_t x2; int32_t y2;
      int32_t x3; int32_t y3;
      get_rect_angle(height, width , ang , &x0, &y0, &x1, &y1, &x2, &y2, &x3, &y3);
      ang=(360+180-ang)%360;
      rect(xc, yc, x0, y0, x1, y1, x2, y2, x3, y3, height, ang, col, soft, true);
      if(col!=colBord){
         rect(xc, yc, x0, y0, x1, y1, x2, y2, x3, y3, height, ang, colBord, soft, false);
      }
   }
*/

   // цветовой градусник
   Color color_gauge(Color colMin, Color colMax, float koef){
       Color ret;
       ret.r=round((koef*((int16_t)colMax.r-colMin.r)))+colMin.r;
       ret.g=round((koef*((int16_t)colMax.g-colMin.g)))+colMin.g;
       ret.b=round((koef*((int16_t)colMax.b-colMin.b)))+colMin.b;
       //ret.w=round((koef*((int16_t)colMax.w-colMin.w)))+colMin.w;
       return ret;
   }       

   // цветовой градусник со средним цветом
   Color color_gauge(Color colMin, Color colMed, Color colMax, float koef){
       Color ret;
       if(koef>0.5) {
          return color_gauge(colMed, colMax, (koef-0.5)*2);
       }
       return color_gauge(colMin, colMed, koef*2);
   }       
};

}  // namespace bp1658cj
}  // namespace esphome