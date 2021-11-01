using System;

//[AttributeUsage(AttributeTargets.Class)]
public class Header : Attribute
{
    public string name;

    public Header(string name)
    {
        this.name = name;
    }
}