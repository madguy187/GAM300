using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Reflection;

namespace Eclipse
{
    public static class HeaderReader
    {
        public static string GetHeaderFromProp(string klassName, string propName)
        {
            Object klass = klassName;
            
            Type myType = typeof(klass);
            MemberInfo[] myMembers = myType.GetMembers();

            for(int i = 0; i < myMembers.Length; i++)
            {
                if (myMembers[i].ToString() == propName)
                {
                    Object[] myAttributes = myMembers[i].GetCustomAttributes(true);

                    if (myAttributes.Length == 0) return String.Empty;
                    else return myAttributes[0].ToString();
                }
            }

            return String.Empty;
        }
    }
}