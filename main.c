#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NIL (0)

#define internal static
#define global static
#define local static

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef i16 b16;
typedef i32 b32;

typedef struct
{
  i32 x;
  i32 y;
  u32 height;
  u32 width;
  u32 border_width;
  i32 window_depth;
  u32 window_class;
  u64 value_mask;

} WindowProperties;

typedef struct
{
  i32 x;
  i32 y;
  i32 z;

} vertex;

void
sleep_ms(long ms)
{
  struct timespec ts;
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms % 1000) * 1000000L;

  while (nanosleep(&ts, &ts))
  {
    NULL;
  }
}

void
move_down(double *y)
{
  ++*y;
}

void
move_up(double *y)
{
  --*y;
}

typedef struct
{
  void (*move)(double *a);
} movement;

int
main()
{
  Display *MainDisplay = XOpenDisplay(NIL);

  Window root = XDefaultRootWindow(MainDisplay);
  int screen = DefaultScreen(MainDisplay);

  Visual *v = DefaultVisual(MainDisplay, screen);

  XSetWindowAttributes wa = {
    .background_pixmap = None,
    .background_pixel = BlackPixel(MainDisplay, DefaultScreen(MainDisplay)),
    .border_pixmap = CopyFromParent,
    .border_pixel = 0,
    .bit_gravity = ForgetGravity,
    .win_gravity = NorthWestGravity,
    .backing_store = NotUseful,
    .backing_planes = 1,
    .backing_pixel = 0,
    .save_under = False,
    .event_mask = {},
    .do_not_propagate_mask = {},
    .override_redirect = False,
    .colormap = CopyFromParent,
    .cursor = None
  };

  i32 dp_heigth = DisplayHeight(MainDisplay, screen);
  i32 dp_width = DisplayWidth(MainDisplay, screen);

  WindowProperties p = {

    .x = dp_width / 2,
    .y = dp_heigth / 2,
    .width = (u32)400,
    .height = (u32)400,
    .border_width = 0,
    .window_depth = CopyFromParent,
    .window_class = CopyFromParent,
    .value_mask = CWBackPixel,

  };

  Window window =
    XCreateWindow(
      MainDisplay,
      root,
      p.x,
      p.y,
      p.width,
      p.height,
      p.border_width,
      p.window_depth,
      p.window_class,
      v,
      p.value_mask,
      &wa);

  XSetWindowBorder(MainDisplay, window, 60);
  XSelectInput(MainDisplay, window, ExposureMask | StructureNotifyMask);
  XMapWindow(MainDisplay, window);

  double x = p.width / 2;
  double y = p.height / 2;
  u32 rect_width = 50;
  u32 rect_height = 50;

  b16 running = 1;

  u64 color = 0x0000ff00;

  GC gc = XCreateGC(MainDisplay, window, 0, NIL);
  GC textGc = XCreateGC(MainDisplay, window, 0, NIL);
  XSetForeground(MainDisplay, gc, color);

  double *pX = &x;
  double *pY = &y;

  movement m = {
    .move = move_down
  };

  while (running)
  {
    if (*pY + rect_height >= p.height)
    {
      m.move = move_up;
    }
    else if (*pY <= 0)
    {
      m.move = move_down;
    }

    char words[] = "working";

    XTextItem ti = {
      .chars = words,
      .nchars = (int)strlen(words),
      .delta = 0,
      .font = None
    };

    XClearWindow(MainDisplay, window);

    XDrawText(
      MainDisplay,
      window,
      textGc,
      50,
      50,
      &ti,
      1);

    XFillRectangle(
      MainDisplay,
      window,
      gc,
      (i32)*pX,
      (i32)*pY,
      rect_height,
      rect_width);

    XFillRectangle(
      MainDisplay,
      window,
      gc,
      (i32)*pX + 100,
      (i32)*pY,
      rect_height,
      rect_width);

    XFillRectangle(
      MainDisplay,
      window,
      gc,
      (i32)*pX - 100,
      (i32)*pY,
      rect_height,
      rect_width);

    m.move(pY);

    sleep_ms(10);

    XFlush(MainDisplay);
  }

  XFreeGC(MainDisplay, gc);
  XFreeGC(MainDisplay, textGc);
  XCloseDisplay(MainDisplay);

  return 0;
}
