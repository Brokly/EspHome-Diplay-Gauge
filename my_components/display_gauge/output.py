import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output, display, color
from esphome.const import (
    CONF_ID,
    CONF_SIZE,
    CONF_WIDTH,
)

CODEOWNERS = ["@Brokly"]
DEPENDENCIES = ["display"]
CONF_DISPLAY = "display"
CONF_RADIUS = "radius"
CONF_START_ANGLE = "start_angle"
CONF_END_ANGLE = "end_angle"
CONF_COLOR = "color"
CONF_START_COLOR = "fill_color"
CONF_MED_COLOR = "half_color"
CONF_END_COLOR = "max_color"
CONF_BORDER_COLOR = "border_color"
CONF_BKG_COLOR = "bkg_color"
CONF_3D_COLOR = "3d_color"
CONF_STYLE_SOFT = "style_soft"
CONF_AUTO_REFRESH = "auto_refresh"
CONF_CENTER_X = "center_x"
CONF_CENTER_Y = "center_y"
CONF_MIRROR = "mirroring"
CONF_KNOB_WIDTH = "knob_width"
CONF_KNOB_SIZE = "knob_size"
CONF_KNOB_SOFT = "khob_soft"

display_gauge_ns = cg.esphome_ns.namespace("display_gauge")
DisplayGauge = display_gauge_ns.class_("DisplayGauge", output.FloatOutput, cg.Component)

def validate_guge(config):
    if CONF_WIDTH in config and (CONF_RADIUS in config or CONF_END_ANGLE in config):
        raise cv.Invalid("For a circle gauge, do not use the 'width' parameter.\nFor a rectangular gauge, do not use the 'radius' and 'end_angle' parameters.\nSimultaneous use of these parameters is not possible.")
    return config

def validate_color(config):
    if CONF_START_COLOR in config and CONF_KNOB_WIDTH not in config and CONF_KNOB_SOFT not in config and CONF_KNOB_SIZE not in config:
          raise cv.Invalid("Fill color ('fill_color') cannot be used without knob size ('knob_size' or 'knob_width' or 'knob_soft')")
    return config

def validate_3d_color(config):
    if CONF_3D_COLOR in config and CONF_BORDER_COLOR not in config:
          raise cv.Invalid("3D effect ('3d_color') is possible only with the parameter 'border_color'")
    return config

CONFIG_SCHEMA = cv.All(
    output.FLOAT_OUTPUT_SCHEMA.extend(
        {
           cv.Required(CONF_ID): cv.declare_id(DisplayGauge),
           cv.Optional(CONF_DISPLAY): cv.use_id(display.DisplayBuffer),
           cv.Required(CONF_CENTER_X): cv.int_range(min=-2147483648, max=2147483647),
           cv.Required(CONF_CENTER_Y): cv.int_range(min=-2147483648, max=2147483647),
           cv.Optional(CONF_SIZE): cv.int_range(min=1, max=32767),
           cv.Optional(CONF_WIDTH): cv.int_range(min=1, max=32767),
           cv.Optional(CONF_RADIUS): cv.int_range(min=1, max=32767),
           cv.Optional(CONF_START_ANGLE): cv.int_range(min=0, max=359),
           cv.Optional(CONF_END_ANGLE): cv.int_range(min=0, max=359),
           cv.Required(CONF_COLOR): cv.use_id(color.ColorStruct),
           cv.Optional(CONF_START_COLOR): cv.use_id(color.ColorStruct),
           cv.Optional(CONF_END_COLOR): cv.use_id(color.ColorStruct),
           cv.Optional(CONF_MED_COLOR): cv.use_id(color.ColorStruct),
           cv.Optional(CONF_BORDER_COLOR): cv.use_id(color.ColorStruct),
           cv.Optional(CONF_3D_COLOR): cv.use_id(color.ColorStruct),
           cv.Optional(CONF_BKG_COLOR): cv.use_id(color.ColorStruct),
           cv.Optional(CONF_KNOB_WIDTH): cv.int_range(min=1, max=32767),
           cv.Optional(CONF_KNOB_SIZE): cv.int_range(min=1, max=32767),
           cv.Optional(CONF_STYLE_SOFT, default=True): cv.boolean,
           cv.Optional(CONF_AUTO_REFRESH, default=True): cv.boolean,
           cv.Optional(CONF_MIRROR, default=False): cv.boolean,
           cv.Optional(CONF_KNOB_SOFT): cv.boolean,
        },
    )
    .extend(cv.COMPONENT_SCHEMA),
    validate_guge,
    validate_color,
    validate_3d_color,
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    if display_config := config.get(CONF_DISPLAY):
        drawing_display = await cg.get_variable(display_config)
        cg.add(var.set_display(drawing_display))
    if CONF_WIDTH in config: 
        cg.add(var.set_width(config[CONF_WIDTH]))
    if CONF_CENTER_X in config:
        cg.add(var.set_x(config[CONF_CENTER_X]))
    if CONF_CENTER_Y in config:
        cg.add(var.set_y(config[CONF_CENTER_Y]))
    if CONF_SIZE in config:
        cg.add(var.set_height(config[CONF_SIZE]))
    if CONF_WIDTH in config:
        cg.add(var.set_width(config[CONF_WIDTH]))
    if CONF_RADIUS in config:
        cg.add(var.set_radius(config[CONF_RADIUS]))
    if CONF_KNOB_WIDTH in config:
        cg.add(var.set_knob_width(config[CONF_KNOB_WIDTH]))
    if CONF_KNOB_SIZE in config:
        cg.add(var.set_knob(config[CONF_KNOB_SIZE]))
    if CONF_START_ANGLE in config:
        cg.add(var.set_ang0(config[CONF_START_ANGLE]))
    if CONF_END_ANGLE in config:
        cg.add(var.set_ang100(config[CONF_END_ANGLE]))
    if CONF_STYLE_SOFT in config:
        cg.add(var.set_soft(config[CONF_STYLE_SOFT]))
    if CONF_KNOB_SOFT in config:
        cg.add(var.set_knob_soft(config[CONF_KNOB_SOFT]))
    if CONF_AUTO_REFRESH in config:
        cg.add(var.set_repaint(config[CONF_AUTO_REFRESH]))
    if CONF_MIRROR in config:
        cg.add(var.set_mirroring(config[CONF_MIRROR]))
    if bk_color_config := config.get(CONF_BKG_COLOR):
        bk_color= await cg.get_variable(bk_color_config)
        cg.add(var.set_bkg_color(bk_color))
    if color0_config := config.get(CONF_COLOR):
        color0= await cg.get_variable(color0_config)
        cg.add(var.set_color0(color0))
    if colorStart_config := config.get(CONF_START_COLOR):
        colorStart= await cg.get_variable(colorStart_config)
        cg.add(var.set_colorStart(colorStart))
    if color100_config := config.get(CONF_END_COLOR):
        color100= await cg.get_variable(color100_config)
        cg.add(var.set_color100(color100))
    if color50_config := config.get(CONF_MED_COLOR):
        color50= await cg.get_variable(color50_config)
        cg.add(var.set_color50(color50))
    if color_bord_config := config.get(CONF_BORDER_COLOR):
        color_bord= await cg.get_variable(color_bord_config)
        cg.add(var.set_border_color(color_bord))
    if color_3d_config := config.get(CONF_3D_COLOR):
        color_3d= await cg.get_variable(color_3d_config)
        cg.add(var.set_3d_color(color_3d))
  


  