# EspHome-Diplay-Gauge
### Gauge-component for visualizing parameters on a graphical display in EspHome 
### Компонент для EspHome, позволяющий нарисовать индикатор "градусник" на дисплее.

![1](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/1eff9829-f386-4a47-89bb-4d98d5f458d3)
![2](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/f605f2eb-de23-47f4-8e51-105672fb7c70)

### Устройство из семейства FloatOutput. 
Для использования разместить папку с комплектом файлов компонента в дополнительной дирректории. В моем случае это папка my_components, лежащая рядом с конфигурацияей (yaml файлом) вашего устройства. А в папке my_components находится папка display_gauge, с комплектом файлов из одноименной папки данного репозитория. Для подключения компонента к проекту вы должны внести в yaml файл следующий блок:

```yaml
external_components:
  - source:
      type: local
      path: my_components
```

После этого вы можете создать любое количество "градусников" на вашем дисплее

```yaml
output:
#градусник
  - platform: display_gauge
    id: test_gauge1_id       # ID градусника
    display: my_display      # Дисплей на котором рисуется этот градусник (не обязательный)
  # основные параметры градусника 
    start_angle: 290         # для спидометра стартовый угол (начало градусника), для прямого градусника угол поворота в градусах от 0 до 359, (не обязательный, по умолчанию, для спидометра:200, для плоского:0)
    end_angle: 250           # конечный угол (конец градусника)  в градусах от 0 до 359, (не обязательный, по умолчанию: 340), признак градусник в форме спидометра, не совместимо с "width"
    size: 11                 # толщина градусника (не обязательный, по умолчанию: 16)
    radius: 54               # радиус градусника-спидометра (не обязательный, по умолчанию: 50), признак градусника в форме спидометра, не совместимо с "width"
    width: 100               # признак плоского градусника, она же ширина, не совместимо с "radius" и "end_angle"
    center_x: 64             # координата X центра дуги градусника (обязательный параметр)
    center_y: 100            # координата Y центра дуги градусника (обязательный параметр)
  # цвета
    color: my_light_blue     # начальный цвет заполнения (обязательный параметр)
    max_color: my_red        # цвет линии заполнения градусника при максимальных показаниях (не обязательный), в отсутствии такой же , как стартовый
    half_color: my_green     # цвет линии заполнения градусника при 50% показаниях (не обязательный) 
    bkg_color: my_light_gray # фоновый цвет заполнения (не обязательный) 
    border_color: my_gray    # цвет бордюра (не обязательный)
    3d_color: my_white       # цвет дополнительного бордюра для 3d эфекта (не обязательный), только при наличии border_color
    fill_color: my_light_green # цвет заполнения линии показаний (не обязательный), только при наличии указателя
  # cтилизация
    style_soft: true         # стиль, при TRUE торцы градусника закруглены (не обязательный) (по умолчанию: true)
    auto_refresh: true       # включает отрисовку при установке нового значения показаний (не обязательный) (по умолчанию: true)
    mirroring: false         # изменяет направление заполнения градусника с "по часовой стрелке" на "против часовой стрелки" (по умолчанию: false)
  # указатель (по умолчанию - круглый)
    knob_size: 24            # радиус круглого указателя или высота прямоугольного указателя (не обязательный)
    knob_width: 10           # ширина прямоугольного указателя, оно же признак квадратного указателя (не обязательный)
    khob_soft: true          # стиль прямоугольного указателя, с закругленными краями он же признак квадратного указателя (не обязательный)
```

### Использование в стиле конфига yaml: 

```yaml
on_...:
  then:
    # установит показания индикатора test_gauge2_id в положение 50%
    - output.set_level: 
        id: test_gauge1_id
        level: 50% #
```

### Использование в лямбдах: 

В примере переменная val содержит показания 25.5. Предполагается, что диапазон отображения градусников от -30.0 до 40.0. Поскольку градусник является наследником float output он принимает значения лежащие в диапазоне от 0.0 до 1.0 . В переменной koef, пересчитанные данные. Используя диапазон отображения приводим показания к требуемому диапазону.

```yaml
   lambda: |-
      float val=25.5;
      float koef=(val+30.0)/(40.0+30.0); // пересчет показаний в диапазон 0.0 ... 1.0
      id(test_gauge1_id).set_level(koef); // установка показаний грдусников      
      id(test_gauge2_id).set_level(koef);
      id(test_gauge2_id).repaint(); // принудительная отрисовка градусника
```

### Примеры конфигурации:

```yaml
    start_angle: 290 
    end_angle: 250
    size: 11
    radius: 54
    center_x: 64
    center_y: 64
    color: my_red
    style_soft: false
```
![image](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/bdbc64d1-a122-4d81-aed8-9ea2e9448087)

```yaml
    start_angle: 290 
    end_angle: 250
    size: 11
    radius: 54
    center_x: 64
    center_y: 64
    color: my_red
    bkg_color: my_blue  #<==
    style_soft: false
```
![image](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/ef91f45c-659f-4a97-8bcf-c6be99cc77d7)

```yaml
    start_angle: 290 
    end_angle: 250
    size: 11
    radius: 54
    center_x: 64
    center_y: 64
    color: my_red
    bkg_color: my_blue
    border_color: my_gray #<==
    style_soft: true      #<==
```
![image](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/b01324e1-8bd4-4214-9af7-9dc3acb52ca5)

```yaml
    start_angle: 0            #<==
    size: 11
    width: 100
    center_x: 64
    center_y: 80              #<==
    color: my_red
    bkg_color: my_light_gray  #<==
    border_color: my_gray
    3d_color: my_white        #<==
    style_soft: true
```
![image](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/4f8af890-153a-4684-b9bb-93b66bcd1ea1)

```yaml
    start_angle: 0 
    size: 11
    width: 100
    center_x: 64
    center_y: 80
    color: my_red
    bkg_color: my_light_gray
    border_color: my_gray
    3d_color: my_white
    style_soft: true         #<==
    knob_size: 8
```
![image](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/d68aa27e-1ac2-4d2e-9fbc-9d11fe1d2f16)

```yaml
    start_angle: 0 
    size: 11
    width: 100
    center_x: 64
    center_y: 80
    color: my_red
    bkg_color: my_light_gray
    border_color: my_gray
    3d_color: my_white
    style_soft: true
    knob_size: 16
    knob_width: 10 
```
![image](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/3c4e3a31-128e-4eb4-990c-06102189cead)

```yaml
    start_angle: 0 
    size: 11
    width: 100
    center_x: 64
    center_y: 80
    color: my_red
    bkg_color: my_light_gray
    border_color: my_gray
    3d_color: my_white
    style_soft: true
    knob_size: 16 
    knob_width: 10
    khob_soft: true
```
![image](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/e7335ae0-61b5-40c9-a6b0-fff8d4a0374f)

```yaml
    start_angle: 45 
    size: 11
    width: 100
    center_x: 64
    center_y: 80
    color: my_red
    max_color: my_blue 
    half_color: my_green 
    bkg_color: my_light_gray
    border_color: my_gray
    3d_color: my_white
    style_soft: true
    mirroring: true
    knob_size: 16 
    knob_width: 10
    khob_soft: true
```
![image](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/9647a4a7-17a9-40f4-b944-f7296ff18972)![image](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/a5397b78-05f8-47cb-84d5-715a551e338b)![image](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/3ce32a63-3d7b-4d87-b9cf-b53788578a6d)

```yaml
    start_angle: 290 
    end_angle: 250
    size: 6
    center_x: 64
    center_y: 64
    color: my_red
    max_color: my_blue 
    half_color: my_green 
    bkg_color: my_light_gray
    border_color: my_gray
    3d_color: my_white
    fill_color: my_light_gray
    style_soft: true
    knob_size: 7
```
![image](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/15d80204-a549-401e-848c-ca9810bae724)

```yaml
    start_angle: 290 
    end_angle: 250
    size: 16
    center_x: 64
    center_y: 64
    color: my_yellow
    bkg_color: my_blue
    border_color: my_gray
    3d_color: my_white
    fill_color: my_red
    style_soft: true
    knob_size: 7 
```
![image](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/228915b1-ac65-4aa9-83db-f0961471ef9d)

```yaml
    start_angle: 290 
    end_angle: 250
    size: 16
    center_x: 64
    center_y: 64
    color: my_green
    bkg_color: my_blue
    fill_color: my_red
    style_soft: true
    knob_size: 8 
```
![image](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/180cdde1-168c-4351-a1a9-ae585c496674)

### Цвета используемые в примерах
```yaml
color:
  - id: my_red
    red: 100%
    green: 0%
    blue: 0%
  - id: my_yellow
    red: 100%
    green: 100%
    blue: 0%
  - id: my_green
    red: 0%
    green: 100%
    blue: 0%
  - id: my_blue
    red: 0%
    green: 0%
    blue: 100%
  - id: my_white
    red: 100%
    green: 100%
    blue: 100%
  - id: my_magenta
    red: 100%
    green: 0%
    blue: 100%
  - id: my_gray
    red: 70%
    green: 70%
    blue: 70%
  - id: my_light_gray
    red: 85%
    green: 85%
    blue: 70%
  - id: my_black
    red: 0%
    green: 0%
    blue: 0%  
  - id: my_light_blue
    red: 0%
    green: 0%
    blue: 80%  
  - id: my_light_green
    red: 0%
    green: 70%
    blue: 0%
```

### Демонстрационные видео

https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/361e95d7-70fe-4103-83b6-468b08ad0fea

https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/1aa98090-4276-42a0-a4c9-082c091b2cf8

