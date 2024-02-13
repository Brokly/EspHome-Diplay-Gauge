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
    #width: 100              # признак плоского градусника, она же ширина, не совместимо с "radius" и "end_angle"
    center_x: 64             # координата X центра дуги градусника (обязательный)
    center_y: 100            # координата Y центра дуги градусника (обязательный)
  # цвета
    color: my_light_blue     # начальный цвет заполнения (обязательный параметр)
    max_color: my_red        # цвет линии заполнения градусника при максимальных показаниях (не обязательный), в отсутствии такой же , как стартовый
    half_color: my_green     # цвет линии заполнения градусника при 50% показаниях (не обязательный) 
    bkg_color: my_light_gray # фоновый цвет заполнения 
    border_color: my_gray    # цвет бордюра (не обязательный)
    3d_color: my_white       # цвет дополнительного бордюра для 3d эфекта (не обязательный), только при наличии border_color
    fill_color: my_light_green # цвет заполнения линии показаний (не обязательный), только при наличии указателя
  # cтилизация
    style_soft: true         # стиль, при TRUE торцы градусника закруглены (не обязательный) (по умолчанию: true)
    auto_refresh: true       # включает отрисовку при установке нового значения показаний (не обязательный) (по умолчанию: true)
    mirroring: false         # изменяет направление заполнения градусника с "по часовой стрелке" на "против часовой стрелки" (по умолчанию: false)
  # указатель
    knob_width: 10           # ширина прямоугольного указателя
    #knob_height: 24         # высота прямоугольного указателя
    #khob_soft: true         # стиль прямоугольного указателя, с закругленными краями
    #knob_radius: 8          # радиус круглого указателя
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








https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/361e95d7-70fe-4103-83b6-468b08ad0fea

