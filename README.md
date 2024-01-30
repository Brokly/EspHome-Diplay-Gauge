# EspHome-Diplay-Gauge
Gauge-component for visualizing parameters on a graphical display in EspHome 
Компонент для EspHome, позволяющий нарисовать индикатор "градусник" на дисплее.

![1](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/1eff9829-f386-4a47-89bb-4d98d5f458d3)
![2](https://github.com/Brokly/EspHome-Diplay-Gauge/assets/11642286/f605f2eb-de23-47f4-8e51-105672fb7c70)

<b>Устройство из семейства FloatOutput.</b><br> 
Для использования разместить папку с комплектом файлов компонента в дополнительной дирректории. В моем случае это папка my_components, лежащая рядом с конфигурацияей (yaml файлом) вашего устройства. А в папке my_coponents находится папка display_gauge, с комплектом файлов из одноименной папки данного репозитория. Для подключения компонента к проекту вы должны внести в yaml файл следующий блок:

external_components:
  - source:
      type: local
      path: my_components

После этого вы можете создать любое количество "градусников" на вашем дисплее
output:
  
  #градусник 1
  - platform: display_gauge
    id: test_gauge1_id       # ID градусника, для дальнейшей его визуализации
    display: my_display      # Дисплей на котором рисуется этот градусник (не обязательно)
    start_angle: 5           # стартовый угол (начало градусника) в градусах от 0 до 359, (не обязательно, по умолчанию: 50)
    end_angle: 120           # конечный угол (конец градусника)  в градусах от 0 до 359, (не обязательно, по умолчанию: 310)
    radius: 56               # радиус градусника (не обязательно, по умолчанию: 50)
    height: 14               # общая ширина градусника (не обязательно, по умолчанию: 14)
    center_x: 64             # координата X центра дуги градусника (обязательно)
    center_y: 64             # координата Y центра дуги градусника (обязательно)
    color: my_blue           # цвет линии заполнения градусника, либо начальный цвет (не обязательно, по умолчанию: белый)
    full_color: my_yellow    # цвет линии заполнения градусника при максимальных показаниях (не обязательно), в отсутствии заполнение будет одного цвета и не будет зависеть от показаний
    border_color: my_white   # цвет бордюра (не обязательно), в отсутствии параметра цвет аналогичный линии заполнения
    style_soft: true         # стиль, при TRUE торцы градусника закруглены (не обязательно) (по умолчанию: true)
    auto_refresh: true       # включает отрисовку при установке нового значения показаний (не обязательно) (по умолчанию: true)
    mirroring: false         # изменяет направление заполнения градусника с "по часовой стрелке" на "против часовой стрелки" (по умолчанию: false)
  
  #градусник 2
  - platform: display_gauge
    id: test_gauge2_id
    display: my_display
    start_angle: 30
    end_angle: 170
    radius: 39
    height: 14
    center_x: 64
    center_y: 64
    full_color: my_red
    color: my_blue
    border_color: my_green    
    style_soft: false
    auto_refresh: true  
    mirroring: true

    <b>Работа в стле конфига yaml:</b>

on_...:
  then:
    # установит показания индикатора test_gauge2_id в положение 50%
    - output.set_level: 
        id: test_gauge2_id
        level: 50% #

В РАБОТЕ....
