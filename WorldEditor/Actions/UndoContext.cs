using System;
using System.Collections.Generic;
using System.Text;

namespace WorldEditor.Actions
{
    public class UndoContext
    {
        public Models.World World { get; }
        private Stack<Action> UndoStack { get; } = new Stack<Action>();
        private Stack<Action> RedoStack { get; } = new Stack<Action>();

        public bool CanUndo => UndoStack.Count > 0;
        public bool CanRedo => RedoStack.Count > 0;

        public UndoContext(Models.World world)
        {
            this.World = world;
        }

        public void ExecuteAction(Action action)
        {
            action.Execute(this.World);
            this.UndoStack.Push(action);
            this.RedoStack.Clear();
        }

        public void UndoAction()
        {
            Action action = UndoStack.Pop();
            action.Undo(this.World);
            RedoStack.Push(action);
        }

        public void RedoAction()
        {
            Action action = RedoStack.Pop();
            action.Execute(this.World);
            UndoStack.Push(action);
        }
    }
}
