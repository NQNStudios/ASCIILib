class Main {
    static function main() {
        Sdl.init(Sdl.INIT_EVERYTHING);
        var win = Sdl.createWindow("Hello", 100, 100, 800, 600, Sdl.WINDOW_OPENGL);
        var ren = Sdl.createRenderer(win, -1, Sdl.RENDERER_ACCELERATED);
        var bmp = Sdl.loadBMP("test.bmp");
        var tex = Sdl.createTextureFromSurface(ren, bmp);
        Sdl.freeSurface(bmp);

        for (i in 0...3) {
            Sdl.renderClear(ren);
            Sdl.renderCopy(ren, tex, cast 0, cast 0);
            Sdl.renderPresent(ren);
            Sdl.delay(1000);
        }
    }
}
