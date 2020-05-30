using System;
using System.IO;
using System.Collections.Generic;
using System.Text;

namespace TypeRegistrator
{
    class Program
    {
        static void Main(string[] args)
        {
            //TODO: pass this via args
            string sourceDirectory = "C:/RegistratorTest";
            string registerationMacro = "REGISTER_TYPE";
            string getRegisteredMacro = "REGISTERED_TYPES";
            string outputFile = "C:/RegistratorTest/output/output.h";

            RegisterTypes(sourceDirectory, registerationMacro, getRegisteredMacro, outputFile);
        }

        static void RegisterTypes(string sourceDirectory, string registerationMacro, string getRegisteredMacro, string outputFile)
        {
            string[] allfiles = Directory.GetFiles(sourceDirectory, "*.h", SearchOption.AllDirectories);
            HashSet<string> types = new HashSet<string>();
            foreach (var file in allfiles)
            {
                if (file.Replace('\\', '/').Equals(outputFile))
                    continue;

                var src = File.ReadAllText(file);
                GatherRegisteredTypes(src, registerationMacro, ref types);
            }

            if (types.Count > 0)
                WriteRegisteredTypes(outputFile, types, getRegisteredMacro);
        }

        static void GatherRegisteredTypes(string src, string tag, ref HashSet<string> types)
        {
            for (int tagIndex = 0; ; tagIndex += tag.Length)
            {
                tagIndex = src.IndexOf(tag, tagIndex);
                if (tagIndex == -1)
                    return;

                int typeStartIndex = tagIndex + tag.Length + 1;
                int typeEndIndex = src.IndexOf(')', typeStartIndex);
                if (typeEndIndex == -1)
                {
                    Console.WriteLine("Error in source code. No closing braket");
                    Environment.Exit(1);
                }

                string type = src.Substring(typeStartIndex, typeEndIndex - typeStartIndex);

                if (!types.Add(type))
                {
                    Console.WriteLine("Error in source code. Type registered more than once");
                    Environment.Exit(2);
                }
            }
        }

        static void WriteRegisteredTypes(string path, HashSet<string> types, string macro)
        {
            //TODO: include headers of registered types
            string src = File.ReadAllText(path);

            StringBuilder newSrc = new StringBuilder(src);

            string macroDefine = "#define " + macro + " ";
            int regIndex = src.IndexOf(macroDefine);

            var typeSetEnumerator = types.GetEnumerator();
            typeSetEnumerator.MoveNext();

            if (regIndex > 0)
            {
                int insertIndex = regIndex + macroDefine.Length;
                //TODO: implement multiline macro
                int endLineIndex = src.IndexOf('\n', insertIndex);
                newSrc.Remove(insertIndex, endLineIndex - insertIndex);
                newSrc.Insert(insertIndex, typeSetEnumerator.Current);
                insertIndex += typeSetEnumerator.Current.Length;
                while (typeSetEnumerator.MoveNext())
                {
                    string insertion = ", " + typeSetEnumerator.Current;
                    newSrc.Insert(insertIndex, insertion);
                    insertIndex += insertion.Length;
                }
            }
            else
            {
                newSrc.Append("\n" + macroDefine);
                newSrc.Append(typeSetEnumerator.Current);
                while (typeSetEnumerator.MoveNext())
                    newSrc.Append(", " + typeSetEnumerator.Current);
            }

            File.WriteAllText(path, newSrc.ToString());
        }
    }
}
