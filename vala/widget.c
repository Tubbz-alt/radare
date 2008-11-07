/*
 *  Grava - General purpose graphing library for Vala
 *  Copyright (C) 2007, 2008  pancake <youterm.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "widget.h"
#include <float.h>
#include <math.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <cairo.h>
#include "node.h"


#define GRAVA_WIDGET_TYPE_WHEEL_ACTION (grava_widget_wheel_action_get_type ())

typedef enum  {
	GRAVA_WIDGET_WHEEL_ACTION_PAN = 0,
	GRAVA_WIDGET_WHEEL_ACTION_ZOOM = 1,
	GRAVA_WIDGET_WHEEL_ACTION_ROTATE = 2
} GravaWidgetWheelAction;



struct _GravaWidgetPrivate {
	GravaWidgetWheelAction wheel_action;
	GtkScrolledWindow* sw;
	GtkMenu* menu;
	double opanx;
	double opany;
	double offx;
	double offy;
	GravaNode* on;
};

#define GRAVA_WIDGET_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GRAVA_TYPE_WIDGET, GravaWidgetPrivate))
enum  {
	GRAVA_WIDGET_DUMMY_PROPERTY
};
GType grava_widget_wheel_action_get_type (void);
#define GRAVA_WIDGET_SIZE 30
#define GRAVA_WIDGET_ZOOM_FACTOR 0.1
static gboolean _grava_widget_expose_gtk_widget_expose_event (GtkDrawingArea* _sender, GdkEventExpose* event, gpointer self);
static gboolean _grava_widget_motion_gtk_widget_motion_notify_event (GtkDrawingArea* _sender, GdkEventMotion* event, gpointer self);
static gboolean _grava_widget_button_release_gtk_widget_button_release_event (GtkDrawingArea* _sender, GdkEventButton* event, gpointer self);
static gboolean _grava_widget_button_press_gtk_widget_button_press_event (GtkDrawingArea* _sender, GdkEventButton* event, gpointer self);
static gboolean _grava_widget_scroll_press_gtk_widget_scroll_event (GtkDrawingArea* _sender, GdkEventScroll* event, gpointer self);
static gboolean _grava_widget_key_press_gtk_widget_key_press_event (GtkScrolledWindow* _sender, GdkEventKey* event, gpointer self);
static gboolean _grava_widget_key_release_gtk_widget_key_release_event (GtkScrolledWindow* _sender, GdkEventKey* event, gpointer self);
static void __lambda0 (GravaWidget* obj, const char* addr, GravaWidget* self);
static void ___lambda0_grava_widget_load_graph_at (GravaWidget* _sender, const char* addr, gpointer self);
static gboolean grava_widget_scroll_press (GravaWidget* self, GtkDrawingArea* da, GdkEventScroll* es);
static gboolean grava_widget_key_release (GravaWidget* self, GtkWidget* w, GdkEventKey* ek);
static gboolean grava_widget_key_press (GravaWidget* self, GtkWidget* w, GdkEventKey* ek);
static void __lambda1 (GtkImageMenuItem* imi, GravaWidget* self);
static void ___lambda1_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static void __lambda2 (GtkImageMenuItem* imi, GravaWidget* self);
static void ___lambda2_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static void __lambda3 (GtkImageMenuItem* imi, GravaWidget* self);
static void ___lambda3_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static void __lambda4 (GtkImageMenuItem* imi, GravaWidget* self);
static void ___lambda4_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static void __lambda5 (GtkImageMenuItem* imi, GravaWidget* self);
static void ___lambda5_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static gboolean grava_widget_button_press (GravaWidget* self, GtkDrawingArea* da, GdkEventButton* eb);
static double grava_widget_abs (GravaWidget* self, double x);
static gboolean grava_widget_button_release (GravaWidget* self, GtkDrawingArea* da, GdkEventButton* em);
static gboolean grava_widget_motion (GravaWidget* self, GtkDrawingArea* da, GdkEventMotion* em);
static gboolean grava_widget_expose (GravaWidget* self, GtkDrawingArea* w, GdkEventExpose* ev);
static GObject * grava_widget_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static gpointer grava_widget_parent_class = NULL;
static void grava_widget_finalize (GObject* obj);




GType grava_widget_wheel_action_get_type (void) {
	static GType grava_widget_wheel_action_type_id = 0;
	if (G_UNLIKELY (grava_widget_wheel_action_type_id == 0)) {
		static const GEnumValue values[] = {{GRAVA_WIDGET_WHEEL_ACTION_PAN, "GRAVA_WIDGET_WHEEL_ACTION_PAN", "pan"}, {GRAVA_WIDGET_WHEEL_ACTION_ZOOM, "GRAVA_WIDGET_WHEEL_ACTION_ZOOM", "zoom"}, {GRAVA_WIDGET_WHEEL_ACTION_ROTATE, "GRAVA_WIDGET_WHEEL_ACTION_ROTATE", "rotate"}, {0, NULL, NULL}};
		grava_widget_wheel_action_type_id = g_enum_register_static ("GravaWidgetWheelAction", values);
	}
	return grava_widget_wheel_action_type_id;
}


#line 45 "widget.vala"
GtkWidget* grava_widget_get_widget (GravaWidget* self) {
	GtkWidget* _tmp0;
#line 45 "widget.vala"
	g_return_val_if_fail (GRAVA_IS_WIDGET (self), NULL);
#line 47 "widget.vala"
	_tmp0 = NULL;
#line 47 "widget.vala"
	return (_tmp0 = GTK_WIDGET (self->priv->sw), (_tmp0 == NULL ? NULL : g_object_ref (_tmp0)));
}


#line 463 "widget.vala"
static gboolean _grava_widget_expose_gtk_widget_expose_event (GtkDrawingArea* _sender, GdkEventExpose* event, gpointer self) {
	return grava_widget_expose (self, _sender, event);
}


#line 419 "widget.vala"
static gboolean _grava_widget_motion_gtk_widget_motion_notify_event (GtkDrawingArea* _sender, GdkEventMotion* event, gpointer self) {
	return grava_widget_motion (self, _sender, event);
}


#line 412 "widget.vala"
static gboolean _grava_widget_button_release_gtk_widget_button_release_event (GtkDrawingArea* _sender, GdkEventButton* event, gpointer self) {
	return grava_widget_button_release (self, _sender, event);
}


#line 367 "widget.vala"
static gboolean _grava_widget_button_press_gtk_widget_button_press_event (GtkDrawingArea* _sender, GdkEventButton* event, gpointer self) {
	return grava_widget_button_press (self, _sender, event);
}


#line 95 "widget.vala"
static gboolean _grava_widget_scroll_press_gtk_widget_scroll_event (GtkDrawingArea* _sender, GdkEventScroll* event, gpointer self) {
	return grava_widget_scroll_press (self, _sender, event);
}


#line 150 "widget.vala"
static gboolean _grava_widget_key_press_gtk_widget_key_press_event (GtkScrolledWindow* _sender, GdkEventKey* event, gpointer self) {
	return grava_widget_key_press (self, _sender, event);
}


#line 132 "widget.vala"
static gboolean _grava_widget_key_release_gtk_widget_key_release_event (GtkScrolledWindow* _sender, GdkEventKey* event, gpointer self) {
	return grava_widget_key_release (self, _sender, event);
}


static void __lambda0 (GravaWidget* obj, const char* addr, GravaWidget* self) {
	g_return_if_fail (GRAVA_IS_WIDGET (obj));
	g_return_if_fail (addr != NULL);
}


static void ___lambda0_grava_widget_load_graph_at (GravaWidget* _sender, const char* addr, gpointer self) {
	__lambda0 (_sender, addr, self);
}


#line 56 "widget.vala"
void grava_widget_create_widgets (GravaWidget* self) {
	GtkDrawingArea* _tmp0;
	GtkScrolledWindow* _tmp3;
	GtkAdjustment* _tmp2;
	GtkAdjustment* _tmp1;
	GtkAdjustment* _tmp5;
	GtkAdjustment* _tmp4;
	GtkViewport* _tmp6;
	GtkViewport* vp;
#line 56 "widget.vala"
	g_return_if_fail (GRAVA_IS_WIDGET (self));
	_tmp0 = NULL;
#line 58 "widget.vala"
	self->da = (_tmp0 = g_object_ref_sink (((GtkDrawingArea*) (gtk_drawing_area_new ()))), (self->da == NULL ? NULL : (self->da = (g_object_unref (self->da), NULL))), _tmp0);
	/* add event listeners */
#line 61 "widget.vala"
	gtk_widget_add_events (GTK_WIDGET (self->da), ((gint) (GDK_BUTTON1_MOTION_MASK | GDK_SCROLL_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK)));
	/*da.set_events(  Gdk.EventMask.BUTTON1_MOTION_MASK );
	 Gdk.EventMask.POINTER_MOTION_MASK );*/
#line 67 "widget.vala"
	g_signal_connect_object (GTK_WIDGET (self->da), "expose-event", ((GCallback) (_grava_widget_expose_gtk_widget_expose_event)), self, 0);
#line 68 "widget.vala"
	g_signal_connect_object (GTK_WIDGET (self->da), "motion-notify-event", ((GCallback) (_grava_widget_motion_gtk_widget_motion_notify_event)), self, 0);
#line 69 "widget.vala"
	g_signal_connect_object (GTK_WIDGET (self->da), "button-release-event", ((GCallback) (_grava_widget_button_release_gtk_widget_button_release_event)), self, 0);
#line 70 "widget.vala"
	g_signal_connect_object (GTK_WIDGET (self->da), "button-press-event", ((GCallback) (_grava_widget_button_press_gtk_widget_button_press_event)), self, 0);
#line 71 "widget.vala"
	g_signal_connect_object (GTK_WIDGET (self->da), "scroll-event", ((GCallback) (_grava_widget_scroll_press_gtk_widget_scroll_event)), self, 0);
	_tmp3 = NULL;
	_tmp2 = NULL;
	_tmp1 = NULL;
#line 73 "widget.vala"
	self->priv->sw = (_tmp3 = g_object_ref_sink (((GtkScrolledWindow*) (gtk_scrolled_window_new ((_tmp1 = g_object_ref_sink (((GtkAdjustment*) (gtk_adjustment_new (((double) (0)), ((double) (10)), ((double) (1000)), ((double) (2)), ((double) (100)), ((double) (1000))))))), (_tmp2 = g_object_ref_sink (((GtkAdjustment*) (gtk_adjustment_new (((double) (0)), ((double) (10)), ((double) (1000)), ((double) (2)), ((double) (100)), ((double) (1000))))))))))), (self->priv->sw == NULL ? NULL : (self->priv->sw = (g_object_unref (self->priv->sw), NULL))), _tmp3);
	(_tmp2 == NULL ? NULL : (_tmp2 = (g_object_unref (_tmp2), NULL)));
	(_tmp1 == NULL ? NULL : (_tmp1 = (g_object_unref (_tmp1), NULL)));
#line 76 "widget.vala"
	gtk_scrolled_window_set_policy (self->priv->sw, GTK_POLICY_NEVER, GTK_POLICY_NEVER);
	_tmp5 = NULL;
	_tmp4 = NULL;
#line 78 "widget.vala"
	_tmp6 = NULL;
	vp = (_tmp6 = g_object_ref_sink (((GtkViewport*) (gtk_viewport_new ((_tmp4 = g_object_ref_sink (((GtkAdjustment*) (gtk_adjustment_new (((double) (0)), ((double) (10)), ((double) (1000)), ((double) (2)), ((double) (100)), ((double) (1000))))))), (_tmp5 = g_object_ref_sink (((GtkAdjustment*) (gtk_adjustment_new (((double) (0)), ((double) (10)), ((double) (1000)), ((double) (2)), ((double) (100)), ((double) (1000))))))))))), (_tmp5 == NULL ? NULL : (_tmp5 = (g_object_unref (_tmp5), NULL))), (_tmp4 == NULL ? NULL : (_tmp4 = (g_object_unref (_tmp4), NULL))), _tmp6);
#line 81 "widget.vala"
	gtk_container_add (GTK_CONTAINER (vp), GTK_WIDGET (self->da));
#line 83 "widget.vala"
	gtk_widget_add_events (GTK_WIDGET (self->priv->sw), ((gint) (GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK)));
#line 84 "widget.vala"
	g_signal_connect_object (GTK_WIDGET (self->priv->sw), "key-press-event", ((GCallback) (_grava_widget_key_press_gtk_widget_key_press_event)), self, 0);
#line 85 "widget.vala"
	g_signal_connect_object (GTK_WIDGET (self->priv->sw), "key-release-event", ((GCallback) (_grava_widget_key_release_gtk_widget_key_release_event)), self, 0);
#line 87 "widget.vala"
	gtk_scrolled_window_add_with_viewport (self->priv->sw, GTK_WIDGET (vp));
#line 89 "widget.vala"
	g_signal_connect_object (self, "load-graph-at", ((GCallback) (___lambda0_grava_widget_load_graph_at)), self, 0);
	(vp == NULL ? NULL : (vp = (g_object_unref (vp), NULL)));
}


/*stdout.printf("HOWHOWHOW "+addr);
 capture mouse motion */
#line 95 "widget.vala"
static gboolean grava_widget_scroll_press (GravaWidget* self, GtkDrawingArea* da, GdkEventScroll* es) {
	GdkScrollDirection _tmp2;
#line 95 "widget.vala"
	g_return_val_if_fail (GRAVA_IS_WIDGET (self), FALSE);
#line 95 "widget.vala"
	g_return_val_if_fail (GTK_IS_DRAWING_AREA (da), FALSE);
#line 97 "widget.vala"
	gtk_widget_grab_focus (GTK_WIDGET (self->priv->sw));
	_tmp2 = (*es).direction;
	if (_tmp2 == GDK_SCROLL_UP)
	do {
		GravaWidgetWheelAction _tmp0;
		_tmp0 = self->priv->wheel_action;
		if (_tmp0 == GRAVA_WIDGET_WHEEL_ACTION_PAN)
		do {
#line 103 "widget.vala"
			self->graph->pany = self->graph->pany + (64);
#line 104 "widget.vala"
			break;
		} while (0); else if (_tmp0 == GRAVA_WIDGET_WHEEL_ACTION_ZOOM)
		do {
#line 106 "widget.vala"
			self->graph->zoom = self->graph->zoom + (GRAVA_WIDGET_ZOOM_FACTOR);
#line 107 "widget.vala"
			break;
		} while (0); else if (_tmp0 == GRAVA_WIDGET_WHEEL_ACTION_ROTATE)
		do {
#line 109 "widget.vala"
			self->graph->angle = self->graph->angle - (0.04);
#line 110 "widget.vala"
			break;
		} while (0);
#line 112 "widget.vala"
		break;
	} while (0); else if (_tmp2 == GDK_SCROLL_DOWN)
	do {
		GravaWidgetWheelAction _tmp1;
		_tmp1 = self->priv->wheel_action;
		if (_tmp1 == GRAVA_WIDGET_WHEEL_ACTION_PAN)
		do {
#line 116 "widget.vala"
			self->graph->pany = self->graph->pany - (64);
#line 117 "widget.vala"
			break;
		} while (0); else if (_tmp1 == GRAVA_WIDGET_WHEEL_ACTION_ZOOM)
		do {
#line 119 "widget.vala"
			self->graph->zoom = self->graph->zoom - (GRAVA_WIDGET_ZOOM_FACTOR);
#line 120 "widget.vala"
			break;
		} while (0); else if (_tmp1 == GRAVA_WIDGET_WHEEL_ACTION_ROTATE)
		do {
#line 122 "widget.vala"
			self->graph->angle = self->graph->angle + (0.04);
#line 123 "widget.vala"
			break;
		} while (0);
#line 125 "widget.vala"
		break;
	} while (0);
#line 128 "widget.vala"
	gtk_widget_queue_draw_area (GTK_WIDGET (da), 0, 0, 5000, 2000);
#line 129 "widget.vala"
	return FALSE;
}


#line 132 "widget.vala"
static gboolean grava_widget_key_release (GravaWidget* self, GtkWidget* w, GdkEventKey* ek) {
	guint _tmp0;
#line 132 "widget.vala"
	g_return_val_if_fail (GRAVA_IS_WIDGET (self), FALSE);
#line 132 "widget.vala"
	g_return_val_if_fail (GTK_IS_WIDGET (w), FALSE);
#line 134 "widget.vala"
	gtk_widget_grab_focus (GTK_WIDGET (self->priv->sw));
	/*stdout.printf("Key released %d (%c)\n", (int)ek.keyval, (int)ek.keyval);*/
	_tmp0 = (*ek).keyval;
	if (_tmp0 == 65507)
	do {
#line 140 "widget.vala"
		self->priv->wheel_action = GRAVA_WIDGET_WHEEL_ACTION_PAN;
#line 141 "widget.vala"
		break;
	} while (0); else if (_tmp0 == 65505)
	do {
#line 143 "widget.vala"
		self->priv->wheel_action = GRAVA_WIDGET_WHEEL_ACTION_PAN;
#line 144 "widget.vala"
		break;
	} while (0);
#line 147 "widget.vala"
	return TRUE;
}


#line 150 "widget.vala"
static gboolean grava_widget_key_press (GravaWidget* self, GtkWidget* w, GdkEventKey* ek) {
	gboolean handled;
	guint _tmp4;
#line 150 "widget.vala"
	g_return_val_if_fail (GRAVA_IS_WIDGET (self), FALSE);
#line 150 "widget.vala"
	g_return_val_if_fail (GTK_IS_WIDGET (w), FALSE);
	handled = TRUE;
	/*DrawingArea da = (DrawingArea)w;*/
#line 154 "widget.vala"
	gtk_widget_grab_focus (GTK_WIDGET (self->priv->sw));
	/* */
#line 157 "widget.vala"
	fprintf (stdout, "Key pressed %d (%c)\n", ((gint) ((*ek).keyval)), ((gint) ((*ek).keyval)));
	_tmp4 = (*ek).keyval;
	if (_tmp4 == 'b' || _tmp4 == 65471)
	do {
		char* _tmp0;
#line 61 "node.vala"
		_tmp0 = NULL;
#line 163 "widget.vala"
		mygrava_bp_at (NULL, (_tmp0 = grava_node_get (grava_graph_selected, "label")));
		_tmp0 = (g_free (_tmp0), NULL);
#line 164 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 'B')
	do {
		char* _tmp2;
		char* _tmp1;
#line 703 "glib-2.0.vapi"
		_tmp2 = NULL;
#line 61 "node.vala"
		_tmp1 = NULL;
#line 167 "widget.vala"
		mygrava_bp_at (NULL, (_tmp2 = g_strdup_printf ("-%s", (_tmp1 = grava_node_get (grava_graph_selected, "label")))));
		_tmp2 = (g_free (_tmp2), NULL);
		_tmp1 = (g_free (_tmp1), NULL);
#line 168 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 'S')
	do {
#line 170 "widget.vala"
		g_signal_emit_by_name (G_OBJECT (self), "run-cmd", "!stepo");
#line 171 "widget.vala"
		g_signal_emit_by_name (G_OBJECT (self), "run-cmd", ".!regs*");
#line 172 "widget.vala"
		g_signal_emit_by_name (G_OBJECT (self), "load-graph-at", "$$");
#line 173 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 's' || _tmp4 == 65476)
	do {
#line 176 "widget.vala"
		g_signal_emit_by_name (G_OBJECT (self), "run-cmd", "!step");
#line 177 "widget.vala"
		g_signal_emit_by_name (G_OBJECT (self), "run-cmd", ".!regs*");
#line 179 "widget.vala"
		g_signal_emit_by_name (G_OBJECT (self), "load-graph-at", "$$");
#line 180 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 65478)
	do {
#line 182 "widget.vala"
		g_signal_emit_by_name (G_OBJECT (self), "run-cmd", "!cont");
#line 183 "widget.vala"
		g_signal_emit_by_name (G_OBJECT (self), "run-cmd", ".!regs*");
#line 184 "widget.vala"
		g_signal_emit_by_name (G_OBJECT (self), "load-graph-at", "$$");
#line 185 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 65507)
	do {
#line 187 "widget.vala"
		self->priv->wheel_action = GRAVA_WIDGET_WHEEL_ACTION_ZOOM;
#line 188 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 65505)
	do {
#line 190 "widget.vala"
		self->priv->wheel_action = GRAVA_WIDGET_WHEEL_ACTION_ROTATE;
#line 191 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 65056)
	do {
#line 206 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 65289)
	do {
#line 208 "widget.vala"
		grava_graph_select_next (self->graph);
#line 209 "widget.vala"
		if (grava_graph_selected == NULL) {
#line 210 "widget.vala"
			grava_graph_select_next (self->graph);
		}
#line 211 "widget.vala"
		if (grava_graph_selected != NULL) {
			/*sw.get_size(ref w, ref, h);
			 XXXX get window size*/
#line 214 "widget.vala"
			self->graph->panx = -grava_graph_selected->x + 350;
#line 215 "widget.vala"
			self->graph->pany = -grava_graph_selected->y + 350;
		}
#line 217 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 65361 || _tmp4 == 'h')
	do {
#line 220 "widget.vala"
		self->graph->panx = self->graph->panx + (GRAVA_WIDGET_S * self->graph->zoom);
#line 221 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 65364 || _tmp4 == 'j')
	do {
#line 224 "widget.vala"
		self->graph->pany = self->graph->pany - (GRAVA_WIDGET_S * self->graph->zoom);
#line 225 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 65362 || _tmp4 == 'k')
	do {
#line 228 "widget.vala"
		self->graph->pany = self->graph->pany + (GRAVA_WIDGET_S * self->graph->zoom);
#line 229 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 65363 || _tmp4 == 'l')
	do {
#line 232 "widget.vala"
		self->graph->panx = self->graph->panx - (GRAVA_WIDGET_S * self->graph->zoom);
#line 233 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 'H')
	do {
#line 235 "widget.vala"
		self->graph->panx = self->graph->panx + (GRAVA_WIDGET_S * self->graph->zoom);
#line 236 "widget.vala"
		if (grava_graph_selected != NULL) {
#line 237 "widget.vala"
			grava_graph_selected->x = grava_graph_selected->x - (GRAVA_WIDGET_S * self->graph->zoom);
		}
#line 238 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 'J')
	do {
#line 240 "widget.vala"
		self->graph->pany = self->graph->pany - (GRAVA_WIDGET_S * self->graph->zoom);
#line 241 "widget.vala"
		if (grava_graph_selected != NULL) {
#line 242 "widget.vala"
			grava_graph_selected->y = grava_graph_selected->y + (GRAVA_WIDGET_S * self->graph->zoom);
		}
#line 243 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 'K')
	do {
#line 245 "widget.vala"
		self->graph->pany = self->graph->pany + (GRAVA_WIDGET_S * self->graph->zoom);
#line 246 "widget.vala"
		if (grava_graph_selected != NULL) {
#line 247 "widget.vala"
			grava_graph_selected->y = grava_graph_selected->y - (GRAVA_WIDGET_S * self->graph->zoom);
		}
#line 248 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 'L')
	do {
#line 250 "widget.vala"
		if (self->priv->wheel_action == GRAVA_WIDGET_WHEEL_ACTION_ZOOM) {
		} else {
#line 252 "widget.vala"
			self->graph->panx = self->graph->panx - (GRAVA_WIDGET_S * self->graph->zoom);
#line 253 "widget.vala"
			if (grava_graph_selected != NULL) {
#line 254 "widget.vala"
				grava_graph_selected->x = grava_graph_selected->x + (GRAVA_WIDGET_S * self->graph->zoom);
			}
		}
#line 256 "widget.vala"
		break;
	} while (0); else if (_tmp4 == '.')
	do {
#line 258 "widget.vala"
		if (grava_graph_selected != NULL) {
			char* _tmp3;
#line 61 "node.vala"
			_tmp3 = NULL;
#line 259 "widget.vala"
			g_signal_emit_by_name (G_OBJECT (self), "load-graph-at", (_tmp3 = grava_node_get (grava_graph_selected, "label")));
			_tmp3 = (g_free (_tmp3), NULL);
		} else {
#line 260 "widget.vala"
			grava_graph_select_next (self->graph);
		}
#line 261 "widget.vala"
		break;
	} while (0); else if (_tmp4 == ':')
	do {
#line 263 "widget.vala"
		g_signal_emit_by_name (G_OBJECT (self), "load-graph-at", "eip");
#line 264 "widget.vala"
		grava_graph_selected = grava_graph_selected = NULL;
#line 265 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 'u')
	do {
#line 267 "widget.vala"
		grava_graph_undo_select (self->graph);
#line 268 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 't')
	do {
#line 270 "widget.vala"
		grava_graph_select_true (self->graph);
#line 271 "widget.vala"
		break;
	} while (0); else if (_tmp4 == 'f')
	do {
#line 273 "widget.vala"
		grava_graph_select_false (self->graph);
#line 274 "widget.vala"
		break;
	} while (0); else if (_tmp4 == '+')
	do {
#line 276 "widget.vala"
		self->graph->zoom = self->graph->zoom + (GRAVA_WIDGET_ZOOM_FACTOR);
#line 277 "widget.vala"
		break;
	} while (0); else if (_tmp4 == '-')
	do {
#line 279 "widget.vala"
		self->graph->zoom = self->graph->zoom - (GRAVA_WIDGET_ZOOM_FACTOR);
#line 280 "widget.vala"
		break;
	} while (0); else
	do {
#line 282 "widget.vala"
		handled = FALSE;
#line 283 "widget.vala"
		break;
	} while (0);
	/*expose(da, ev);*/
#line 287 "widget.vala"
	gtk_widget_queue_draw_area (GTK_WIDGET (self->da), 0, 0, 5000, 2000);
#line 289 "widget.vala"
	return TRUE;
}


static void __lambda1 (GtkImageMenuItem* imi, GravaWidget* self) {
	g_return_if_fail (GTK_IS_IMAGE_MENU_ITEM (imi));
}


static void ___lambda1_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	__lambda1 (_sender, self);
}


#line 292 "widget.vala"
void grava_widget_do_popup_generic (GravaWidget* self) {
	GtkImageMenuItem* imi;
	GtkMenu* _tmp0;
	GtkImageMenuItem* _tmp1;
#line 292 "widget.vala"
	g_return_if_fail (GRAVA_IS_WIDGET (self));
	imi = NULL;
	_tmp0 = NULL;
#line 295 "widget.vala"
	self->priv->menu = (_tmp0 = g_object_ref_sink (((GtkMenu*) (gtk_menu_new ()))), (self->priv->menu == NULL ? NULL : (self->priv->menu = (g_object_unref (self->priv->menu), NULL))), _tmp0);
	_tmp1 = NULL;
#line 296 "widget.vala"
	imi = (_tmp1 = g_object_ref_sink (((GtkImageMenuItem*) (gtk_image_menu_item_new_from_stock ("gtk-zoom-in", NULL)))), (imi == NULL ? NULL : (imi = (g_object_unref (imi), NULL))), _tmp1);
#line 297 "widget.vala"
	g_signal_connect_object (GTK_MENU_ITEM (imi), "activate", ((GCallback) (___lambda1_gtk_menu_item_activate)), self, 0);
	/* foo */
#line 300 "widget.vala"
	gtk_menu_shell_append (GTK_MENU_SHELL (self->priv->menu), ((GtkWidget*) (GTK_MENU_ITEM (imi))));
#line 301 "widget.vala"
	gtk_widget_show_all (GTK_WIDGET (self->priv->menu));
#line 302 "widget.vala"
	gtk_menu_popup (self->priv->menu, NULL, NULL, NULL, NULL, ((guint) (0)), ((guint) (0)));
	(imi == NULL ? NULL : (imi = (g_object_unref (imi), NULL)));
}


static void __lambda2 (GtkImageMenuItem* imi, GravaWidget* self) {
	char* _tmp0;
	g_return_if_fail (GTK_IS_IMAGE_MENU_ITEM (imi));
	/*stdout.printf("go in!\n");*/
#line 61 "node.vala"
	_tmp0 = NULL;
#line 314 "widget.vala"
	core_load_graph_at_label (NULL, (_tmp0 = grava_node_get (grava_graph_selected, "label")));
	_tmp0 = (g_free (_tmp0), NULL);
}


static void ___lambda2_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	__lambda2 (_sender, self);
}


static void __lambda3 (GtkImageMenuItem* imi, GravaWidget* self) {
	char* _tmp0;
	g_return_if_fail (GTK_IS_IMAGE_MENU_ITEM (imi));
	/*stdout.printf("add bp!\n");*/
#line 61 "node.vala"
	_tmp0 = NULL;
#line 324 "widget.vala"
	mygrava_bp_at (NULL, (_tmp0 = grava_node_get (grava_graph_selected, "label")));
	_tmp0 = (g_free (_tmp0), NULL);
}


static void ___lambda3_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	__lambda3 (_sender, self);
}


static void __lambda4 (GtkImageMenuItem* imi, GravaWidget* self) {
	char* _tmp0;
	g_return_if_fail (GTK_IS_IMAGE_MENU_ITEM (imi));
#line 61 "node.vala"
	_tmp0 = NULL;
#line 330 "widget.vala"
	mygrava_bp_rm_at (NULL, (_tmp0 = grava_node_get (grava_graph_selected, "label")));
	_tmp0 = (g_free (_tmp0), NULL);
}


static void ___lambda4_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	__lambda4 (_sender, self);
}


static void __lambda5 (GtkImageMenuItem* imi, GravaWidget* self) {
	g_return_if_fail (GTK_IS_IMAGE_MENU_ITEM (imi));
	/*stdout.printf("FUCKME: \n"+imi.submenu_placement());*/
#line 356 "widget.vala"
	g_signal_emit_by_name (G_OBJECT (self), "load-graph-at", gtk_label_get_text ((GTK_LABEL (GTK_BIN (imi)->child))));
}


static void ___lambda5_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	__lambda5 (_sender, self);
}


#line 305 "widget.vala"
void grava_widget_do_popup_menu (GravaWidget* self) {
	GtkImageMenuItem* imi;
	GtkMenu* _tmp0;
	GtkImageMenuItem* _tmp1;
	GtkImageMenuItem* _tmp2;
	GtkImageMenuItem* _tmp3;
#line 305 "widget.vala"
	g_return_if_fail (GRAVA_IS_WIDGET (self));
	imi = NULL;
	_tmp0 = NULL;
#line 308 "widget.vala"
	self->priv->menu = (_tmp0 = g_object_ref_sink (((GtkMenu*) (gtk_menu_new ()))), (self->priv->menu == NULL ? NULL : (self->priv->menu = (g_object_unref (self->priv->menu), NULL))), _tmp0);
	/*imi = new ImageMenuItem.with_label("Focus");*/
	_tmp1 = NULL;
#line 311 "widget.vala"
	imi = (_tmp1 = g_object_ref_sink (((GtkImageMenuItem*) (gtk_image_menu_item_new_from_stock ("gtk-zoom-in", NULL)))), (imi == NULL ? NULL : (imi = (g_object_unref (imi), NULL))), _tmp1);
#line 312 "widget.vala"
	g_signal_connect_object (GTK_MENU_ITEM (imi), "activate", ((GCallback) (___lambda2_gtk_menu_item_activate)), self, 0);
	/*MenuItem mi = menu.get_active();
	load_graph_at(((Label)imi.child).get_text()); //"0x400");
	stdout.printf(" cocococo "+ menu.);*/
#line 319 "widget.vala"
	gtk_menu_shell_append (GTK_MENU_SHELL (self->priv->menu), ((GtkWidget*) (GTK_MENU_ITEM (imi))));
	_tmp2 = NULL;
#line 321 "widget.vala"
	imi = (_tmp2 = g_object_ref_sink (((GtkImageMenuItem*) (gtk_image_menu_item_new_with_label ("Breakpoint here")))), (imi == NULL ? NULL : (imi = (g_object_unref (imi), NULL))), _tmp2);
#line 322 "widget.vala"
	g_signal_connect_object (GTK_MENU_ITEM (imi), "activate", ((GCallback) (___lambda3_gtk_menu_item_activate)), self, 0);
#line 326 "widget.vala"
	gtk_menu_shell_append (GTK_MENU_SHELL (self->priv->menu), ((GtkWidget*) (GTK_MENU_ITEM (imi))));
	_tmp3 = NULL;
#line 328 "widget.vala"
	imi = (_tmp3 = g_object_ref_sink (((GtkImageMenuItem*) (gtk_image_menu_item_new_with_label ("Remove breakpoint")))), (imi == NULL ? NULL : (imi = (g_object_unref (imi), NULL))), _tmp3);
#line 329 "widget.vala"
	g_signal_connect_object (GTK_MENU_ITEM (imi), "activate", ((GCallback) (___lambda4_gtk_menu_item_activate)), self, 0);
#line 332 "widget.vala"
	gtk_menu_shell_append (GTK_MENU_SHELL (self->priv->menu), ((GtkWidget*) (GTK_MENU_ITEM (imi))));
	/* TODO: add continue until here
	
	imi = new ImageMenuItem.with_label("Remove true branch");
	imi.activate += imi => {
	///stdout.printf("Focus!\n");
	};
	menu.append(imi);
	
	imi = new ImageMenuItem.with_label("Remove false branch");
	imi.activate += imi => {
	//stdout.printf("Focus!\n");
	};
	menu.append(imi);
	*/
#line 349 "widget.vala"
	if (grava_graph_selected != NULL) {
		GtkSeparatorMenuItem* _tmp4;
		_tmp4 = NULL;
#line 350 "widget.vala"
		gtk_menu_shell_append (GTK_MENU_SHELL (self->priv->menu), ((GtkWidget*) (GTK_MENU_ITEM ((_tmp4 = g_object_ref_sink (((GtkSeparatorMenuItem*) (gtk_separator_menu_item_new ()))))))));
		(_tmp4 == NULL ? NULL : (_tmp4 = (g_object_unref (_tmp4), NULL)));
		{
			GSList* str_collection;
			GSList* str_it;
#line 352 "widget.vala"
			str_collection = grava_graph_selected->calls;
			for (str_it = str_collection; str_it != NULL; str_it = str_it->next) {
				const char* _tmp6;
				char* str;
#line 25 "node.vala"
				_tmp6 = NULL;
#line 352 "widget.vala"
				str = (_tmp6 = ((char*) (str_it->data)), (_tmp6 == NULL ? NULL : g_strdup (_tmp6)));
				{
					GtkImageMenuItem* _tmp5;
					_tmp5 = NULL;
#line 353 "widget.vala"
					imi = (_tmp5 = g_object_ref_sink (((GtkImageMenuItem*) (gtk_image_menu_item_new_with_label (str)))), (imi == NULL ? NULL : (imi = (g_object_unref (imi), NULL))), _tmp5);
#line 354 "widget.vala"
					g_signal_connect_object (GTK_MENU_ITEM (imi), "activate", ((GCallback) (___lambda5_gtk_menu_item_activate)), self, 0);
					/*"0x400");*/
#line 358 "widget.vala"
					gtk_menu_shell_append (GTK_MENU_SHELL (self->priv->menu), ((GtkWidget*) (GTK_MENU_ITEM (imi))));
					str = (g_free (str), NULL);
				}
			}
		}
	}
#line 362 "widget.vala"
	gtk_widget_show_all (GTK_WIDGET (self->priv->menu));
	/*menu.popup(null, null, null, null, eb.button, 0);*/
#line 364 "widget.vala"
	gtk_menu_popup (self->priv->menu, NULL, NULL, NULL, NULL, ((guint) (0)), ((guint) (0)));
	(imi == NULL ? NULL : (imi = (g_object_unref (imi), NULL)));
}


#line 367 "widget.vala"
static gboolean grava_widget_button_press (GravaWidget* self, GtkDrawingArea* da, GdkEventButton* eb) {
	GravaNode* _tmp0;
	GravaNode* n;
	gboolean _tmp1;
#line 367 "widget.vala"
	g_return_val_if_fail (GRAVA_IS_WIDGET (self), FALSE);
#line 367 "widget.vala"
	g_return_val_if_fail (GTK_IS_DRAWING_AREA (da), FALSE);
	/*EventButton eb = event.button;
	EventMotion em = event.motion; */
#line 371 "widget.vala"
	_tmp0 = NULL;
	n = (_tmp0 = grava_graph_click (self->graph, (*eb).x - self->graph->panx, (*eb).y - self->graph->pany), (_tmp0 == NULL ? NULL : g_object_ref (_tmp0)));
#line 373 "widget.vala"
	gtk_widget_grab_focus (GTK_WIDGET (self->priv->sw));
#line 374 "widget.vala"
	grava_graph_selected = n;
#line 375 "widget.vala"
	if ((*eb).button == 3) {
#line 376 "widget.vala"
		if (n != NULL) {
#line 377 "widget.vala"
			grava_widget_do_popup_menu (self);
		} else {
#line 378 "widget.vala"
			grava_widget_do_popup_generic (self);
		}
	}
#line 380 "widget.vala"
	if (n != NULL) {
#line 381 "widget.vala"
		if ((((*eb).y - (16 * self->graph->zoom) - self->graph->pany) < (n->y * self->graph->zoom)) && ((*eb).x - self->graph->panx > (n->x + n->w - (16 * self->graph->zoom)) * self->graph->zoom)) {
#line 383 "widget.vala"
			n->has_body = !n->has_body;
#line 384 "widget.vala"
			grava_node_fit (n);
		}
#line 387 "widget.vala"
		self->priv->opanx = (*eb).x;
#line 388 "widget.vala"
		self->priv->opany = (*eb).y;
		/*graph.draw(Gdk.cairo_create(da.window));*/
#line 391 "widget.vala"
		gtk_widget_queue_draw_area (GTK_WIDGET (da), 0, 0, 5000, 3000);
	}
#line 394 "widget.vala"
	return (_tmp1 = TRUE, (n == NULL ? NULL : (n = (g_object_unref (n), NULL))), _tmp1);
}


#line 404 "widget.vala"
static double grava_widget_abs (GravaWidget* self, double x) {
#line 404 "widget.vala"
	g_return_val_if_fail (GRAVA_IS_WIDGET (self), 0.0);
#line 406 "widget.vala"
	if (x > 0) {
#line 407 "widget.vala"
		return x;
	}
#line 408 "widget.vala"
	return -x;
}


#line 412 "widget.vala"
static gboolean grava_widget_button_release (GravaWidget* self, GtkDrawingArea* da, GdkEventButton* em) {
	GravaNode* _tmp0;
#line 412 "widget.vala"
	g_return_val_if_fail (GRAVA_IS_WIDGET (self), FALSE);
#line 412 "widget.vala"
	g_return_val_if_fail (GTK_IS_DRAWING_AREA (da), FALSE);
	_tmp0 = NULL;
#line 414 "widget.vala"
	self->priv->on = (_tmp0 = NULL, (self->priv->on == NULL ? NULL : (self->priv->on = (g_object_unref (self->priv->on), NULL))), _tmp0);
#line 415 "widget.vala"
	self->priv->opanx = self->priv->opany = ((double) (0));
#line 416 "widget.vala"
	return TRUE;
}


#line 419 "widget.vala"
static gboolean grava_widget_motion (GravaWidget* self, GtkDrawingArea* da, GdkEventMotion* em) {
	GravaNode* _tmp0;
	GravaNode* n;
	gboolean _tmp3;
#line 419 "widget.vala"
	g_return_val_if_fail (GRAVA_IS_WIDGET (self), FALSE);
#line 419 "widget.vala"
	g_return_val_if_fail (GTK_IS_DRAWING_AREA (da), FALSE);
#line 421 "widget.vala"
	_tmp0 = NULL;
	n = (_tmp0 = grava_graph_selected, (_tmp0 == NULL ? NULL : g_object_ref (_tmp0)));
	/*graph.click(em.x-graph.panx, em.y-graph.pany);*/
#line 422 "widget.vala"
	gtk_widget_grab_focus (GTK_WIDGET (self->priv->sw));
#line 423 "widget.vala"
	if (n != NULL) {
		/* drag node 
		 TODO: properly handle the graph.zoom */
#line 426 "widget.vala"
		if (n != self->priv->on) {
			GravaNode* _tmp2;
			GravaNode* _tmp1;
#line 427 "widget.vala"
			self->priv->offx = ((*em).x - n->x);
#line 428 "widget.vala"
			self->priv->offy = ((*em).y - n->y);
			_tmp2 = NULL;
#line 429 "widget.vala"
			_tmp1 = NULL;
#line 429 "widget.vala"
			self->priv->on = (_tmp2 = (_tmp1 = n, (_tmp1 == NULL ? NULL : g_object_ref (_tmp1))), (self->priv->on == NULL ? NULL : (self->priv->on = (g_object_unref (self->priv->on), NULL))), _tmp2);
		}
#line 431 "widget.vala"
		n->x = ((*em).x) - (self->priv->offx);
#line 432 "widget.vala"
		n->y = ((*em).y) - (self->priv->offy);
#line 434 "widget.vala"
		self->priv->offx = ((*em).x - n->x);
#line 435 "widget.vala"
		self->priv->offy = ((*em).y - n->y);
		/*offx += (offx/graph.zoom);
		offy += (offy/graph.zoom);
		n.x += (offx-(offx*graph.zoom));
		n.y += (offy-(offy*graph.zoom));
		n.x*=graph.zoom;
		n.y*=graph.zoom;*/
#line 444 "widget.vala"
		gtk_widget_queue_draw_area (GTK_WIDGET (da), 0, 0, 5000, 3000);
#line 445 "widget.vala"
		grava_graph_selected = n;
	} else {
		/* pan view */
#line 448 "widget.vala"
		if ((self->priv->opanx != 0) && (self->priv->opany != 0)) {
			double x;
			double y;
			x = (*em).x - self->priv->opanx;
			y = (*em).y - self->priv->opany;
#line 451 "widget.vala"
			self->graph->panx = self->graph->panx + (x);
			/**0.8;*/
#line 452 "widget.vala"
			self->graph->pany = self->graph->pany + (y);
			/**0.8;
			graph.draw(Gdk.cairo_create(da.window));*/
#line 454 "widget.vala"
			gtk_widget_queue_draw_area (GTK_WIDGET (da), 0, 0, 5000, 3000);
		}
		/*Graph.selected = null;*/
#line 457 "widget.vala"
		self->priv->opanx = (*em).x;
#line 458 "widget.vala"
		self->priv->opany = (*em).y;
	}
#line 460 "widget.vala"
	return (_tmp3 = TRUE, (n == NULL ? NULL : (n = (g_object_unref (n), NULL))), _tmp3);
}


#line 463 "widget.vala"
static gboolean grava_widget_expose (GravaWidget* self, GtkDrawingArea* w, GdkEventExpose* ev) {
	GtkDrawingArea* _tmp0;
	GtkDrawingArea* da;
	gboolean _tmp1;
#line 463 "widget.vala"
	g_return_val_if_fail (GRAVA_IS_WIDGET (self), FALSE);
#line 463 "widget.vala"
	g_return_val_if_fail (GTK_IS_DRAWING_AREA (w), FALSE);
#line 465 "widget.vala"
	_tmp0 = NULL;
	da = (_tmp0 = GTK_DRAWING_AREA (w), (_tmp0 == NULL ? NULL : g_object_ref (_tmp0)));
#line 466 "widget.vala"
	grava_widget_draw (self);
#line 467 "widget.vala"
	return (_tmp1 = TRUE, (da == NULL ? NULL : (da = (g_object_unref (da), NULL))), _tmp1);
}


#line 470 "widget.vala"
void grava_widget_draw (GravaWidget* self) {
	cairo_t* ctx;
#line 470 "widget.vala"
	g_return_if_fail (GRAVA_IS_WIDGET (self));
	ctx = gdk_cairo_create (GDK_DRAWABLE (GTK_WIDGET (self->da)->window));
#line 473 "widget.vala"
	if (self->graph->zoom < 0.2) {
#line 474 "widget.vala"
		self->graph->zoom = 0.2;
	}
#line 475 "widget.vala"
	grava_graph_draw (self->graph, ctx);
	(ctx == NULL ? NULL : (ctx = (cairo_destroy (ctx), NULL)));
}


#line 25 "widget.vala"
GravaWidget* grava_widget_construct (GType object_type) {
	GravaWidget * self;
	self = g_object_newv (object_type, 0, NULL);
	return self;
}


#line 25 "widget.vala"
GravaWidget* grava_widget_new (void) {
#line 25 "widget.vala"
	return grava_widget_construct (GRAVA_TYPE_WIDGET);
}


static GObject * grava_widget_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GravaWidgetClass * klass;
	GObjectClass * parent_class;
	GravaWidget * self;
	klass = GRAVA_WIDGET_CLASS (g_type_class_peek (GRAVA_TYPE_WIDGET));
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = GRAVA_WIDGET (obj);
	{
		GravaGraph* _tmp0;
		_tmp0 = NULL;
#line 51 "widget.vala"
		self->graph = (_tmp0 = grava_graph_new (), (self->graph == NULL ? NULL : (self->graph = (g_object_unref (self->graph), NULL))), _tmp0);
#line 52 "widget.vala"
		grava_graph_update (self->graph);
#line 53 "widget.vala"
		grava_widget_create_widgets (self);
	}
	return obj;
}


static void grava_widget_class_init (GravaWidgetClass * klass) {
	grava_widget_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (GravaWidgetPrivate));
	G_OBJECT_CLASS (klass)->constructor = grava_widget_constructor;
	G_OBJECT_CLASS (klass)->finalize = grava_widget_finalize;
	g_signal_new ("load_graph_at", GRAVA_TYPE_WIDGET, G_SIGNAL_RUN_LAST, 0, NULL, NULL, g_cclosure_marshal_VOID__STRING, G_TYPE_NONE, 1, G_TYPE_STRING);
	g_signal_new ("breakpoint_at", GRAVA_TYPE_WIDGET, G_SIGNAL_RUN_LAST, 0, NULL, NULL, g_cclosure_marshal_VOID__STRING, G_TYPE_NONE, 1, G_TYPE_STRING);
	g_signal_new ("run_cmd", GRAVA_TYPE_WIDGET, G_SIGNAL_RUN_LAST, 0, NULL, NULL, g_cclosure_marshal_VOID__STRING, G_TYPE_NONE, 1, G_TYPE_STRING);
}


static void grava_widget_instance_init (GravaWidget * self) {
	self->priv = GRAVA_WIDGET_GET_PRIVATE (self);
	self->priv->wheel_action = GRAVA_WIDGET_WHEEL_ACTION_PAN;
	self->priv->opanx = ((double) (0));
	self->priv->opany = ((double) (0));
	self->priv->offx = ((double) (0));
	self->priv->offy = ((double) (0));
	self->priv->on = NULL;
}


static void grava_widget_finalize (GObject* obj) {
	GravaWidget * self;
	self = GRAVA_WIDGET (obj);
	(self->da == NULL ? NULL : (self->da = (g_object_unref (self->da), NULL)));
	(self->graph == NULL ? NULL : (self->graph = (g_object_unref (self->graph), NULL)));
	(self->priv->sw == NULL ? NULL : (self->priv->sw = (g_object_unref (self->priv->sw), NULL)));
	(self->priv->menu == NULL ? NULL : (self->priv->menu = (g_object_unref (self->priv->menu), NULL)));
	(self->priv->on == NULL ? NULL : (self->priv->on = (g_object_unref (self->priv->on), NULL)));
	G_OBJECT_CLASS (grava_widget_parent_class)->finalize (obj);
}


GType grava_widget_get_type (void) {
	static GType grava_widget_type_id = 0;
	if (grava_widget_type_id == 0) {
		static const GTypeInfo g_define_type_info = { sizeof (GravaWidgetClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) grava_widget_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GravaWidget), 0, (GInstanceInitFunc) grava_widget_instance_init, NULL };
		grava_widget_type_id = g_type_register_static (G_TYPE_OBJECT, "GravaWidget", &g_define_type_info, 0);
	}
	return grava_widget_type_id;
}



