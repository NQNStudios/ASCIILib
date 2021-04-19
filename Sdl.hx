import cpp.ConstCharStar;

@:include("./SDLSupport.h")
@:buildXml('
    <target id="haxe">
        <flag value="-mwindows"/>
        <flag value="-L${MINGW_ROOT}/lib" />
        <flag value="-Dmain=SDL_main"/>
        <lib name="-lmingw32"/>
        <lib name="-lSDL2main"/>
        <lib name="-lSDL2"/>
    </target>'
)
extern class Sdl {
    @:native("SDL_INIT_EVERYTHING")
    static var INIT_EVERYTHING(default,null):InitFlag;


    inline static var WINDOW_NONE:CreateWindowFlag = cast 0;

    @:native("SDL_WINDOW_OPENGL")
    static var WINDOW_OPENGL(default,null):CreateWindowFlag;


    inline static var RENDERER_NONE:CreateRendererFlag = cast 0;

    @:native("SDL_RENDERER_ACCELERATED")
    static var RENDERER_ACCELERATED(default,null):CreateRendererFlag;


    @:native("SDL_Init")
    static function init(flags:InitFlag):Int;

    @:native("SDL_CreateWindow")
    static function createWindow(title:ConstCharStar, x:Int, y:Int, w:Int, h:Int, flags:CreateWindowFlag):WindowPointer;

    @:native("SDL_CreateRenderer")
    static function createRenderer(win:WindowPointer, index:Int, flags:CreateRendererFlag):RendererPointer;

    @:native("SDL_LoadBMP")
    static function loadBMP(file:ConstCharStar):SurfacePointer;

    @:native("SDL_CreateTextureFromSurface")
    static function createTextureFromSurface(renderer:RendererPointer, surface:SurfacePointer):TexturePointer;

    @:native("SDL_FreeSurface")
    static function freeSurface(surface:SurfacePointer):Void;

    @:native("SDL_RenderClear")
    static function renderClear(renderer:RendererPointer):Int;

    @:native("SDL_RenderCopy")
    static function renderCopy(renderer:RendererPointer, texture:TexturePointer, srcRect:RectPointer, dstRect:RectPointer):Int;

    @:native("SDL_RenderPresent")
    static function renderPresent(renderer:RendererPointer):Void;

    @:native("SDL_Delay")
    static function delay(ms:cpp.UInt32):Void;
}

abstract InitFlag(cpp.UInt32) {
    @:op(A|B) static function _(a:InitFlag, b:InitFlag):InitFlag;
}

abstract CreateWindowFlag(cpp.UInt32) {
    @:op(A|B) static function _(a:CreateWindowFlag, b:CreateWindowFlag):WindowPointer;
}

abstract CreateRendererFlag(cpp.UInt32) {
    @:op(A|B) static function _(a:CreateRendererFlag, b:CreateRendererFlag):CreateRendererFlag;
}

@:native("::cpp::Reference<SDL_Window>")
extern class WindowPointer {}

@:native("::cpp::Reference<SDL_Renderer>")
extern class RendererPointer {}

@:native("::cpp::Reference<SDL_Surface>")
extern class SurfacePointer {}

@:native("::cpp::Reference<SDL_Texture>")
extern class TexturePointer {}

@:native("::cpp::Reference<SDL_Rect>")
extern class RectPointer {}
