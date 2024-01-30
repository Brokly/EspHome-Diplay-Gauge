import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output, display, color
from esphome.const import (
    CONF_ID,
    CONF_HEIGHT,
)

CODEOWNERS = ["@Brokly"]
DEPENDENCIES = ["display"]
CONF_DISPLAY = "display"
CONF_RADIUS = "radius"
CONF_START_ANGLE = "start_angle"
CONF_END_ANGLE = "end_angle"
CONF_START_COLOR = "color"
CONF_END_COLOR = "full_color"
CONF_BORDER_COLOR = "border_color"
CONF_STYLE_SOFT = "style_soft"
CONF_AUTO_REFRESH = "auto_refresh"
CONF_CENTER_X = "center_x"
CONF_CENTER_Y = "center_y"
CONF_MIRROR = "mirroring"

display_gauge_ns = cg.esphome_ns.namespace("display_gauge")
DisplayGauge = display_gauge_ns.class_("DisplayGauge", output.FloatOutput, cg.Component)

CONFIG_SCHEMA = cv.All(
    output.FLOAT_OUTPUT_SCHEMA.extend(
        {
           cv.Required(CONF_ID): cv.declare_id(DisplayGauge),
           cv.Optional(CONF_DISPLAY): cv.use_id(display.DisplayBuffer),
           cv.Required(CONF_CENTER_X): cv.int_range(min=-32768, max=32767),
           cv.Required(CONF_CENTER_Y): cv.int_range(min=-32768, max=32767),
           cv.Optional(CONF_HEIGHT, default=14): cv.int_range(min=1, max=2048),
           cv.Optional(CONF_RADIUS, default=50): cv.int_range(min=1, max=1024),
           cv.Optional(CONF_START_ANGLE, default=310): cv.int_range(min=0, max=359),
           cv.Optional(CONF_END_ANGLE, default=50): cv.int_range(min=0, max=359),
           cv.Optional(CONF_START_COLOR): cv.use_id(color.ColorStruct),
           cv.Optional(CONF_END_COLOR): cv.use_id(color.ColorStruct),
           cv.Optional(CONF_BORDER_COLOR): cv.use_id(color.ColorStruct),
           cv.Optional(CONF_STYLE_SOFT, default=True): cv.boolean,
           cv.Optional(CONF_AUTO_REFRESH, default=True): cv.boolean,
           cv.Optional(CONF_MIRROR, default=False): cv.boolean,
        }
    ).extend(cv.COMPONENT_SCHEMA),
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    if display_config := config.get(CONF_DISPLAY):
        drawing_display = await cg.get_variable(display_config)
        cg.add(var.set_display(drawing_display))
    if CONF_CENTER_X in config:
        cg.add(var.set_x(config[CONF_CENTER_X]))
    if CONF_CENTER_Y in config:
        cg.add(var.set_y(config[CONF_CENTER_Y]))
    if CONF_HEIGHT in config:
        cg.add(var.set_height(config[CONF_HEIGHT]))
    if CONF_RADIUS in config:
        cg.add(var.set_radius(config[CONF_RADIUS]))
    if CONF_START_ANGLE in config:
        cg.add(var.set_ang0(config[CONF_START_ANGLE]))
    if CONF_END_ANGLE in config:
        cg.add(var.set_ang100(config[CONF_END_ANGLE]))
    if CONF_STYLE_SOFT in config:
        cg.add(var.set_soft(config[CONF_STYLE_SOFT]))
    if CONF_AUTO_REFRESH in config:
        cg.add(var.set_repaint(config[CONF_AUTO_REFRESH]))
    if CONF_MIRROR in config:
        cg.add(var.set_mirroring(config[CONF_MIRROR]))
    if color100_config := config.get(CONF_END_COLOR):
        color100= await cg.get_variable(color100_config)
        cg.add(var.set_color100(color100))
    if color0_config := config.get(CONF_START_COLOR):
        color0= await cg.get_variable(color0_config)
        cg.add(var.set_color0(color0))
    if color_bord_config := config.get(CONF_BORDER_COLOR):
        color_bord= await cg.get_variable(color_bord_config)
        cg.add(var.set_border_color(color_bord))
    