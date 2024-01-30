#pragma once

#include "esphome/components/display/display.h"
#include "esphome/components/output/float_output.h"

namespace esphome {
namespace display_gauge {

static const char *const TAG = "DisplayGauge";

class DisplayGauge : public output::FloatOutput, public Component {
 public:
  
  //DisplayGauge() {}
  
  // ПАРAМЕТРЫ
  // дисплей на котором рисуем
  void set_display(display::Display *display) { this->display_ = display;}
  // установка координат градусника ( центр расположения)
  void set_x(int16_t x){this->xc_=x;} //cv.int_range(min=-32768, max=32767)
  void set_y(int16_t y){this->yc_=y;} 
  // ширина градусника
  void set_height(uint16_t height){this->height_=height;} 
  // радиус градусника
  void set_radius(uint16_t radius){this->radius_=radius;} 
  // углы раскрытия градусника
  void set_ang0(uint16_t ang){this->ang0_=ang;} // стартовый угол //cv.int_range(min=0, max=359)
  void set_ang100(uint16_t ang){this->ang100_=ang;} // конечный угол
  // минимальный и максимальный цвета  
  void set_color0(Color cl){  // стартовый цвет
      this->col0_=cl; 
      if(this->colset==false){
         this->colset=true;
         this->col100_=cl;
      }
  }
  void set_color100(Color cl){ // конечный цвет
      this->col100_=cl; 
      if(this->colset==false){
         this->colset=true;
         this->col0_=cl;
      }
  }
  void set_border_color(Color cl){ // цвет бордюра
     colbord_=cl; // цвет бордюра
     borderAliterColor=true; // флаг бордюра альтернативного цвета
  }
  // установка стиля
  void set_soft(bool val){this->soft_=val;}
  // признак автообновленя
  void set_repaint(bool val){this->auto_repaint_=val;}
  // признак обратного отсчета (против часовой стрелки)
  void set_mirroring(bool val){this->mirror_=val;}
  
  void setup() override {};
  void loop() override {};

  void dump_config() override{
     ESP_LOGCONFIG(TAG, "DISPLAY GAUGE:");
     ESP_LOGCONFIG(TAG, "  Coordintes X:%d, Y:%d", this->xc_, this->yc_);
     ESP_LOGCONFIG(TAG, "  Height:%u, Radius:%u", this->height_, this->radius_);
     ESP_LOGCONFIG(TAG, "  Angles start:%u, end:%u", this->ang0_, this->ang100_);
     if(this->col0_==this->col100_){
        ESP_LOGCONFIG(TAG, "  Mono color:0x%X", this->col0_.raw_32);
     } else {        
        ESP_LOGCONFIG(TAG, "  Colors start:0x%X, end:0x%X", this->col0_.raw_32, this->col100_.raw_32);
     }
     if(borderAliterColor){
        ESP_LOGCONFIG(TAG, "  Border alterntive color:0x%X", this->colbord_.raw_32);
     }
     ESP_LOGCONFIG(TAG, "  Soft style:", YESNO(this->soft_));
     ESP_LOGCONFIG(TAG, "  Auto repaint:", YESNO(this->auto_repaint_));
     ESP_LOGCONFIG(TAG, "  Mirroring:", YESNO(this->mirror_));
  } 

  float get_setup_priority() const override { return esphome::setup_priority::LATE;}

  
  // рисование градусника
  void repaint(){
     if(display_ != nullptr){
        Color col = this->col0_;
        if(this->col0_ != this->col100_){
           col=color_gauge(this->col100_, this->col0_, 1.0-val_); //определяем цвет печати
        }
        // стандартный способ развертки дуги по часовой стрелке 50->150=260гр , 150->50=100гр
        // окаймляющая дуга всегда рисуется стандартным способом развертки
        // для стандартного режима:
        // начало дуги заполнения всегда должно лежать в начале окаймляющей дуги (ang_0)
        // конец дуги заполнения должен быть между началом и концом, таких вариантов два
        // длинна окаймляющей дуги в градусах рассчитывается для разных случаев по разному
        int16_t ang;
        if(this->mirror_){ // обратный (зеркальный, против часовой стрелки) градусник
           if(this->ang100_ > this->ang0_){ //вычисление угла конечной точки
              ang=(int16_t)(ang0_-((1.0-this->val_)*(360-ang100_+ang0_))+360)%360;      
           } else {
              ang=(this->val_*(ang0_-ang100_))+ang100_;      
           }
           filled_bres_arc(this->xc_, this->yc_, this->radius_, ang100_, ang, col, this->height_, this->soft_); // сначала рисуем заполнение
        } else {
           if(this->ang100_ > this->ang0_){ //вычисление угла конечной точки
              ang=(int16_t)(ang0_-(this->val_*(360-ang100_+ang0_))+360)%360;      
           } else {
              ang=((1.0-this->val_)*(ang0_-ang100_))+ang100_;      
           }
           filled_bres_arc(this->xc_, this->yc_, this->radius_, ang, ang0_, col, this->height_, this->soft_); // сначала рисуем заполнение
        }
        if(this->borderAliterColor){ // бордюр альтернативного цвета
           col=colbord_;
        }
        bres_arc(this->xc_, this->yc_, this->radius_, this->ang100_, this->ang0_, col, this->height_, this->soft_); // потом рисуем контур
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

  display::Display* display_ = nullptr;
  int16_t xc_=0; // координаты центра градусника
  int16_t yc_=0;
  uint16_t height_=5; // высота градусника
  uint16_t radius_=30; // радиус градусника
  uint16_t ang100_=50; // углы раскрытия
  uint16_t ang0_=360-50; 
  float val_=0.0;
  bool mirror_=false; // зеркалировать (градусник растет против часовой стрелки)
  Color col0_={255,255,255};
  bool colset=false; // признак установки начального или конечного цвета
  Color col100_={255,255,255};
  Color colbord_={255,255,255}; // цвет бордюра
  bool borderAliterColor=false; // флаг бордюра альтернативного цвета
  bool soft_=false;
  bool auto_repaint_=true; // автоматическая перерисовка при установке значения
  
      //функция определения осьмушки, к которой принадлежит угол    
    inline uint8_t octant(uint16_t angle){
      return (angle%360)/45;
    }

    //процедура определения координат точки дуги по углу
    inline void coord(uint16_t radius, uint16_t angle, int16_t* x, int16_t* y){
      float ang = ((float)angle * PI)/180.0; //преобразование градусов в радианы
      *y=(int16_t)(round((float)radius*cos(ang)));
      *x=(int16_t)(round((float)radius*sin(ang)));
    }

    //процедура коррекции координат для осьмушки
    //k - номер осьмушки (от 0 до 7)
    //x, y - координаты точки в 7 осьмушке
    //xout, yout - преобразованные координаты для осьмушки номер k
    inline void sim(int16_t x, int16_t y, uint8_t k, int16_t* xout, int16_t* yout){
       if     (k==0)  {*xout =  y; *yout =  x;}
       else if(k==1)  {*xout =  x; *yout =  y;}
       else if(k==2)  {*xout =  x; *yout = -y;}
       else if(k==3)  {*xout =  y; *yout = -x;}
       else if(k==4)  {*xout = -y; *yout = -x;}
       else if(k==5)  {*xout = -x; *yout = -y;}
       else if(k==6)  {*xout = -x; *yout =  y;}
       else /*(k==7)*/{*xout = -y; *yout =  x;}
    }
   
   //процедура рисования точки
   //xc, yc - координаты центра окружности, которой принадлежит дуга
   //x, y - координаты рисования точки дуги относительно центра окружности
   //disp - дисплей на котором рисуем
   //cl - цвет точек
   //till - признак работы в повернутых координатах
   //sizePoint - размер точки
   void point(int16_t xc, int16_t yc, int16_t x, int16_t y, Color cl, bool till=false, uint8_t sizePoint=1){
      if(till){
         int16_t temp=x;
         x=y;
         y=-temp;
      }
      if(sizePoint==2){
         display_->draw_pixel_at(xc+x, yc+y, cl); //рисуем точку
         display_->draw_pixel_at(xc+x+1, yc+y+1, cl); //рисуем точку
         display_->draw_pixel_at(xc+x, yc+y+1, cl); //рисуем точку
         display_->draw_pixel_at(xc+x+1, yc+y, cl); //рисуем точку
      } else if(sizePoint>2){
         uint8_t temp=sizePoint/2;
         display_->rectangle(xc+x-temp, yc+y-temp, sizePoint, sizePoint, cl);
         //display_->filled_circle(xc+x, yc+y, sizePoint/2, cl); // рисуем окружностями
      } else {
         display_->draw_pixel_at(xc+x, yc+y, cl); //рисуем точку
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
   //#define PIX // способ рисования линии
   #define DLT 20 // размер неудобного сектора в градусах
   void bres_arc_ex(int16_t xc, int16_t yc, uint16_t r, uint16_t a, uint16_t b, Color color, uint8_t sizePnt=1, bool first=true){
      bool till=false; 
      a%=360; // нормализвия углов
      b%=360;  
      if(first){
         if((b>90-DLT && b<90+DLT) || (b>270-DLT && b<270+DLT)){ // конечная точка в неудобном секторе
            if(a>360-DLT || a<DLT || (a>180-DLT && a<180+DLT)){ // и начальная точка в неудобном секторе
               int16_t c=getMid(a,b);
               bres_arc_ex(xc, yc, r, a, c, color, sizePnt, false); // первую половину сектора выводим без вращения координат
               a=c; // корректровка начала следующего полусектора, для вывода с поворотом коордиинат
            }
            till=true;
         } else if((a>90-DLT && a<90+DLT) || (a>270-DLT && a<270+DLT)){ // начальная точка в неудобном секторе
            if(b>360-DLT || b<DLT || (b>180-DLT && b<180+DLT)){ // и конечная точка в неудобном секторе
               int16_t c=getMid(a,b);
               bres_arc_ex(xc, yc, r, c, b, color, sizePnt, false); // вторую половину сектора выводим без вращения координат
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
      int16_t xa; int16_t ya;
      coord( r, a, &xa, &ya); //вычисление координат начала дуги
      int16_t xb; int16_t yb;
      coord( r, b, &xb, &yb); //вычисление координат конца дуги
      int16_t x = r; //координаты начальной точки осьмушки, 
      int16_t y = 0; //в которой происходит растровая развёртка
      int16_t xo; int16_t yo;
      int8_t o;
      #ifdef PIX
         int16_t d = 3 - 2 * x; //начальное значение отклонения (этот алгоритм HE рисует смежные пиксели)
      #else   
         int16_t d = 1 - 2 * x;//начальное значение отклонения (этот алгоритм рисует смежные пиксели)
         int16_t er = 0;
      #endif
      while(y<=x){//цикл растровой развёртки, выполняется, пока не развернём всю осьмушку)
         if (oa!=ob || a>b){ //если дуга лежит более более чем в одной осьмушке, то 
            o = oa; //рисуем от начала дуги
            sim(x, y, oa, &xo, &yo); //вычисляем координаты точки для осьмушки с началом дуги
            if ((yo<0 && xo<=xa) || (yo>0 && xo>=xa)) { //если точка принадлежит дуге, то
              point(xc, yc, xo, yo, color, till, sizePnt);        
            }
            o = (o+1) % 8; //номер следующей осьмушки
            while (o != ob){ //пока не достигли осьмушки с концом дуги, рисуем осьмушки целиком
               sim(x, y, o, &xo, &yo); //координаты точки для текущей осьмушки
               point(xc, yc, xo, yo, color, till, sizePnt);        
               o = (o+1) % 8; //номер следующей осьмушки
            }
            sim(x, y, ob, &xo, &yo); //координаты точки для осьмушки с концом дуги
            if ((yo<0 && xo>=xb) || (yo>0 && xo<=xb)){ //если точка принадлежит дуге, то 
               point(xc, yc, xo, yo, color, till, sizePnt);        
            }
               
         } else { // если лежит в одном секторе
            sim(x, y, oa, &xo, &yo); //координаты точки
            if (((yo<0 && xo<=xa) || (yo>=0 && xo>=xa)) && //если точка принадлежит дуге, то
                ((yo<0 && xo>=xb) || (yo>=0 && xo<=xb))) {
               point(xc, yc, xo, yo, color, till, sizePnt);        
            }
         }
         #ifdef PIX
            // этот алгоритм не рисует смежные пиксели
            d += d < 0 ? 4 * y + 6 : 4 * (y - x--) + 10;
            y++;
         #else         
            // этот алгоритм рисует смежные пиксели
            if ((d < 0) && (er <= 0)){
              d += 2 * ++y + 1;
            } else if ((d > 0) && (er > 0)){
              d -= 2 * --x + 1;
            } else {
              d += 2 * (++y - --x);
            }
         #endif
      }
   }      

   //дуга без заливки
   //xc, yc - координаты центра окружности, которой принадлежит дуга
   //r - радиус дуги
   //a - угол начала дуги в градусах
   //b - угол конца дуги в градусах
   //disp - дисплей на котором рисуем
   //color - цвет точек
   //size - ширина фигуры
   //soft - признак закругленных концов дуги
   void bres_arc(int16_t xc, int16_t yc, uint16_t r, uint16_t a, uint16_t b, Color color, uint8_t size=1, bool soft=true){
      if(size<3){
         bres_arc_ex(xc, yc, r, a, b, color, size);   
      } else {
         int16_t xa; int16_t ya;
         int16_t xb; int16_t yb;
         uint16_t size2=size/2;
         r-=size2;
         bres_arc(xc, yc, r, a, b, color);
         uint16_t rb=r+size;
         bres_arc(xc, yc, rb, a, b, color);
         coord(++r, a, &xa, &ya);
         coord(++rb, a, &xb, &yb);
         if(soft){  //закругление начала
            int16_t xcen=xc+((xa+xb)/2);
            int16_t ycen=yc+((ya+yb)/2);
            bres_arc_ex(xcen, ycen, size2, a+180, a, color);
         } else { //рисуем торцевую линию начала
            display_->line(xb+xc, yb+yc, xa+xc, ya+yc, color); 
         }
         coord(r, b, &xa, &ya);
         coord(rb, b, &xb, &yb);
         if(soft){ //закругление торца
            int16_t xcen=xc+((xa+xb)/2);
            int16_t ycen=yc+((ya+yb)/2);
            bres_arc_ex(xcen, ycen, size2, b, b+180, color);
         } else { //рисуем торцевую линию конца
            display_->line(xb+xc, yb+yc, xa+xc, ya+yc, color);
         }
      }
   }    
   
   //дуга с заливкой
   //xc, yc - координаты центра окружности, которой принадлежит дуга
   //r - радиус дуги
   //a - угол начала дуги в градусах
   //b - угол конца дуги в градусах
   //color - цвет точек
   //size - толшина фигуры
   //soft - закругленные края
   void filled_bres_arc(int16_t xc, int16_t yc, uint16_t r, uint16_t a, uint16_t b, Color color, uint8_t size, bool soft=true){
      uint16_t size2=size/2;
      r-=size2;
      uint16_t ro=r;
      if(soft){
        int16_t xa; int16_t ya;
        int16_t xb; int16_t yb;
        uint16_t rb=++r+size;
        coord(r, a, &xa, &ya);
        coord(rb, a, &xb, &yb);
        display_->filled_circle(xc+((xa+xb)/2), yc+((ya+yb)/2), size2, color); // рисуем окружность на торце 
        coord(r, b, &xa, &ya);
        coord(rb, b, &xb, &yb);
        display_->filled_circle(xc+((xa+xb)/2), yc+((ya+yb)/2), size2, color); // рисуем окружность на торце 
      }
      for(uint8_t i=0; i<size; i++){
         bres_arc(xc, yc, ro++, a, b, color,1+(i%2));
      }
   }

   // цветовой градусник
   Color color_gauge(Color colMin, Color colMax, float koef){
       Color ret;
       ret.r=(koef*(colMax.r-colMin.r))+colMin.r;
       ret.g=(koef*(colMax.g-colMin.g))+colMin.g;
       ret.b=(koef*(colMax.b-colMin.b))+colMin.b;
       return ret;
   }       
  
};

}  // namespace bp1658cj
}  // namespace esphome