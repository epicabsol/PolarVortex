using System;
using System.Collections.Generic;
using System.Text;
using WorldEditor.Models;

namespace WorldEditor.Actions
{
    public class ActionGroup : Action
    {
        public List<Action> Actions { get; }
        public string Description { get; }

        public ActionGroup(List<Action> actions, string description = null)
        {
            this.Actions = actions;
            this.Description = description;
        }

        public override void Execute(World world)
        {
            foreach (Action action in this.Actions)
            {
                action.Execute(world);
            }
        }

        public override void Undo(World world)
        {
            for (int i = this.Actions.Count - 1; i >= 0; i--)
            {
                this.Actions[i].Undo(world);
            }
        }

        public override string ToString()
        {
            return Description ?? String.Join(", ", this.Actions);
        }
    }
}
