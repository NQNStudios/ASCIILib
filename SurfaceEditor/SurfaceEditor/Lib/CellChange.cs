using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SurfaceEditor
{
    public struct CellChange
    {
        public int X;
        public int Y;

        public char OldCharacter;
        public char NewCharacter;

        public Color OldBackgroundColor;
        public Color NewBackgroundColor;

        public Color OldCharacterColor;
        public Color NewCharacterColor;

        public bool OldOpacity;
        public bool NewOpacity;

        public string OldSpecialInfo;
        public string NewSpecialInfo;
    }
}
