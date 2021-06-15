namespace Eclipse
{
    abstract class IScriptable
    {
        public abstract void Update();
        public abstract void Start();
        public interface void FixedUpdate();
        public interface void Terminate();
    }
}