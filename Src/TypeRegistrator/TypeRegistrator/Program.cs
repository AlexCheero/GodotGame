using System;
using System.IO;
using System.Collections.Generic;
using System.Text;

namespace TypeRegistrator
{
    //TODO: exclude registrator exe from gitignore
    //TODO: run exe on project build prestep
    class Program
    {
        static void Main(string[] args)
        {
            //TODO: pass this via args
            //string registerationMacro = "DECLARE_REGISTERED_TAG";
            //string getRegisteredMacro = "REGISTERED_TAGS";

            string registerationMacro = "REGISTER_COMPONENT";
            string getRegisteredMacro = "REGISTERED_COMPONENTS";

            string sourceDirectory = "C:/Users/Alex/Documents/GodotProjects/Projects/Prototype/Src/Prototype";
            string outputFile = "C:/Users/Alex/Documents/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/RegisteredTypes.h";
            string registerationMacroMacroDefinitionFile = "C:/Users/Alex/Documents/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/ComponentsMeta.h";

            sourceDirectory.Replace('\\', '/');
            outputFile.Replace('\\', '/');
            registerationMacroMacroDefinitionFile.Replace('\\', '/');

            string[] fileExcludes = new string[] { outputFile, registerationMacroMacroDefinitionFile };

            RegisterTypes(sourceDirectory, registerationMacro, getRegisteredMacro, outputFile, fileExcludes);
        }

        static bool Exclude(string file, string[] excludes)
        {
            foreach (var exclude in excludes)
            {
                if (file.Equals(exclude))
                    return true;
            }

            return false;
        }

        static void RegisterTypes(string sourceDirectory, string registerationMacro, string getRegisteredMacro, string outputFile, string[] excludes)
        {
            string[] allfiles = Directory.GetFiles(sourceDirectory, "*.h", SearchOption.AllDirectories);
            HashSet<Tuple<string, string>> types = new HashSet<Tuple<string, string>>();

            for (int i = 0; i < allfiles.Length; i++)
            {
                string file = allfiles[i].Replace('\\', '/');
                if (Exclude(file, excludes))
                    continue;

                var src = File.ReadAllText(file);
                GatherRegisteredTypes(src, registerationMacro, file, ref types);
            }

            if (types.Count > 0)
            {
                //TODO: make it via FileStream and fix not sonsistent line endings
                WriteHeaders(outputFile, types);
                WriteRegisteredTypes(outputFile, types, getRegisteredMacro);
            }
        }

        static int GetTypeEndIndex(string src, int typeStartIndex)
        {
            char[] closingChars = new char[] { ')', ',' };
            int typeEndIndex = -1;
            foreach (var closingChar in closingChars)
            {
                int closingCharIndex = src.IndexOf(closingChar, typeStartIndex);
                if (typeEndIndex < 0)
                    typeEndIndex = closingCharIndex;
                else if (closingCharIndex > 0 && closingCharIndex < typeEndIndex)
                    typeEndIndex = closingCharIndex;
            }

            if (typeEndIndex < 0)
            {
                Console.WriteLine("Error in source code. No closing braket");
                Environment.Exit(1);
            }

            return typeEndIndex;
        }

        static void GatherRegisteredTypes(string src, string tag, string filePath, ref HashSet<Tuple<string, string>> types)
        {
            for (int tagIndex = 0; ; tagIndex += tag.Length)
            {
                tagIndex = src.IndexOf(tag, tagIndex);
                if (tagIndex == -1)
                    return;

                int typeStartIndex = tagIndex + tag.Length + 1;
                int typeEndIndex = GetTypeEndIndex(src, typeStartIndex);
                //int typeEndIndex = src.IndexOf(')', typeStartIndex);
                //if (typeEndIndex == -1)
                //{
                //    Console.WriteLine("Error in source code. No closing braket");
                //    Environment.Exit(1);
                //}

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
            
            int insertIndex = GetHeaderInsertIndex(newSrc);

            var typeSetEnumerator = types.GetEnumerator();
            while (typeSetEnumerator.MoveNext())
            {
                string include = "#include \"" + typeSetEnumerator.Current.Item1 + "\"";
                if (newSrc.ToString().IndexOf(include) < 0)
                    newSrc.Insert(insertIndex, include + '\n');
            }

            File.WriteAllText(path, newSrc.ToString());
        }

        static int GetHeaderInsertIndex(StringBuilder newSrc)
        {
            string pragma = "#pragma once";
            string srcStr = newSrc.ToString();
            int insertIndex = srcStr.IndexOf(pragma);

            if (insertIndex < 0)
            {
                Console.WriteLine("Error in source code. No #pragma once directive");
                Environment.Exit(3);
            }

            insertIndex += pragma.Length;
            if (srcStr.Length < insertIndex)
            {
                Console.WriteLine("Something went horribly wrong!");
                Environment.Exit(4);
            }

            if (srcStr.Length == insertIndex)
                newSrc.Insert(insertIndex, '\n');
            srcStr = newSrc.ToString();

            if (srcStr[insertIndex] != '\n')
                newSrc.Insert(insertIndex, '\n');
            newSrc.Insert(insertIndex, '\n');

            insertIndex += 2;

            return insertIndex;
        }
    }
}
