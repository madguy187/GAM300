using System;

public class Header : Attribute
{
    public string name;

    public Header(string name)
    {
        this.name = name;
    }
}