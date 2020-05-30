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
            HashSet<Tuple<string, string>> types = new HashSet<Tuple<string, string>>();

            for (int i = 0; i < allfiles.Length; i++)
            {
                string file = allfiles[i].Replace('\\', '/');
                if (file.Equals(outputFile))
                    continue;

                var src = File.ReadAllText(file);
                GatherRegisteredTypes(src, registerationMacro, file, ref types);
            }

            if (types.Count > 0)
            {
                WriteHeaders(outputFile, types);
                WriteRegisteredTypes(outputFile, types, getRegisteredMacro);
            }
        }

        static void GatherRegisteredTypes(string src, string tag, string filePath, ref HashSet<Tuple<string, string>> types)
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

                if (!types.Add(Tuple.Create(filePath, type)))
                {
                    Console.WriteLine("Error in source code. Type registered more than once");
                    Environment.Exit(2);
                }
            }
        }

        static void WriteRegisteredTypes(string path, HashSet<Tuple<string, string>> types, string macro)
        {
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
                if (endLineIndex < 0)
                    newSrc.Remove(insertIndex, src.Length - insertIndex);
                else
                    newSrc.Remove(insertIndex, endLineIndex - insertIndex);

                string currentType = typeSetEnumerator.Current.Item2;
                newSrc.Insert(insertIndex, currentType);
                insertIndex += currentType.Length;

                while (typeSetEnumerator.MoveNext())
                {
                    currentType = typeSetEnumerator.Current.Item2;
                    string insertion = ", " + currentType;
                    newSrc.Insert(insertIndex, insertion);
                    insertIndex += insertion.Length;
                }
            }
            else
            {
                newSrc.Append("\n" + macroDefine);
                string currentType = typeSetEnumerator.Current.Item2;
                newSrc.Append(currentType);
                while (typeSetEnumerator.MoveNext())
                {
                    currentType = typeSetEnumerator.Current.Item2;
                    newSrc.Append(", " + currentType);
                }
            }

            File.WriteAllText(path, newSrc.ToString());
        }

        static void WriteHeaders(string path, HashSet<Tuple<string, string>> types)
        {
            StringBuilder newSrc = new StringBuilder(File.ReadAllText(path));
            newSrc.Insert(0, '\n');

            var typeSetEnumerator = types.GetEnumerator();
            while (typeSetEnumerator.MoveNext())
            {
                string include = "#include \"" + typeSetEnumerator.Current.Item1 + "\"";
                if (newSrc.ToString().IndexOf(include) < 0)
                    newSrc.Insert(0, include + '\n');
            }

            File.WriteAllText(path, newSrc.ToString());
        }
    }
}
