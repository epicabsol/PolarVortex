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
        private Stack<List<Action>> GroupStack { get; } = new Stack<List<Action>>();

        public bool CanUndo => !this.IsInActionGroup && this.UndoStack.Count > 0;
        public bool CanRedo => !this.IsInActionGroup && this.RedoStack.Count > 0;
        public bool IsInActionGroup => this.GroupStack.Count > 0;

        public UndoContext(Models.World world)
        {
            this.World = world;
        }

        public void DoAction(Action action, bool execute = true)
        {
            if (execute)
            {
                action.Execute(this.World);
            }
            if (this.IsInActionGroup)
            {
                this.GroupStack.Peek().Add(action);
            }
            else
            {
                this.UndoStack.Push(action);
                this.RedoStack.Clear();
            }
        }

        public void UndoAction()
        {
            if (this.IsInActionGroup)
            {
#if DEBUG
                throw new InvalidOperationException("Cannot undo when recording an action group.");
#else
                System.Diagnostics.Debug.WriteLine("WARNING: Should not undo while recording an action group. Aborting & rolling back the pending action group.");
                while (this.IsInActionGroup)
                {
                    this.AbortGroup();
                }
#endif
            }
            else
            {
                Action action = UndoStack.Pop();
                action.Undo(this.World);
                RedoStack.Push(action);
            }
        }

        public void RedoAction()
        {
            if (this.IsInActionGroup)
            {
                throw new InvalidOperationException("Cannot redo when recording an action group.");
            }
            else
            {
                Action action = RedoStack.Pop();
                action.Execute(this.World);
                UndoStack.Push(action);
            }
        }

        public void BeginGroup()
        {
            this.GroupStack.Push(new List<Action>());
        }

        public void EndGroup()
        {
            List<Action> actions = this.GroupStack.Pop();
            if (actions.Count > 0)
            {
                ActionGroup group = new ActionGroup(actions);
                this.DoAction(group, false);
            }
        }

        public void AbortGroup()
        {
            List<Action> actions = this.GroupStack.Pop();
            for (int i = actions.Count - 1; i >= 0; i--)
            {
                actions[i].Undo(this.World);
            }
        }
    }
}
