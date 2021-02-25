using System;
using System.Collections.Generic;
using System.Text;

namespace WorldEditor.Actions
{
    public abstract class Action
    {
        public abstract void Execute(Models.World world);
        public abstract void Undo(Models.World world);

        public abstract override string ToString();
    }
}
