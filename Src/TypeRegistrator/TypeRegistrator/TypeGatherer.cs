using System;
using System.Collections.Generic;
using System.IO;

namespace TypeRegistrator
{
    class TypeGatherer
    {
        public void GatherRegisteredTypes(string sourceDirectory, string setMacro, string outputFile, string[] excludes,
            HashSet<string> headers, HashSet<string> types)
        {
            string[] allfiles = Directory.GetFiles(sourceDirectory, "*.h", SearchOption.AllDirectories);

            //TODO: remove test code
            Console.WriteLine("Excludes:");
            for (int i = 0; i < excludes.Length; i++)
                Console.WriteLine("    " + excludes[i]);
            Console.WriteLine("Files:");
            for (int i = 0; i < allfiles.Length; i++)
                Console.WriteLine("    " + allfiles[i].Replace('\\', '/'));

            for (int i = 0; i < allfiles.Length; i++)
            {
                string file = allfiles[i].Replace('\\', '/');
                if (Exclude(file, excludes))
                    continue;

                if (GatherFromSource(file, setMacro, types))
                    headers.Add(GetRelativeHeaderPath(outputFile, file));
            }
        }

        private bool Exclude(string file, string[] excludes)
        {
            foreach (var exclude in excludes)
            {
                if (file.Equals(exclude))
                {
                    //TODO: remove test code
                    Console.WriteLine("exclude!");
                    return true;
                }
            }

            return false;
        }

        private bool GatherFromSource(string file, string tag, HashSet<string> types)
        {
            var src = File.ReadAllText(file);

            bool anyTypeGathered = false;

            for (int tagIndex = 0; ; tagIndex += tag.Length)
            {
                tagIndex = src.IndexOf(tag, tagIndex);
                if (tagIndex == -1)
                    break;

                anyTypeGathered = true;

                int typeStartIndex = tagIndex + tag.Length + 1;
                int typeEndIndex = GetTypeEndIndex(src, typeStartIndex);

                string type = src.Substring(typeStartIndex, typeEndIndex - typeStartIndex);
                if (!types.Add(type))
                {
                    Console.WriteLine("Error in source code. Type registered more than once");
                    Environment.Exit(2);
                }
            }

            return anyTypeGathered;
        }

        private int GetTypeEndIndex(string src, int typeStartIndex)
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

        //TODO: take ../ into account
        private string GetRelativeHeaderPath(string path, string headerPath)
        {
            var splittedPath = path.Split('/');
            var splittedHeaderPath = headerPath.Split('/');

            int firstDifferentPathPartIndex = 0;
            for (; firstDifferentPathPartIndex < splittedPath.Length; firstDifferentPathPartIndex++)
            {
                //TODO: assert index stuff

                var pathPart = splittedPath[firstDifferentPathPartIndex];
                var headerPathPart = splittedHeaderPath[firstDifferentPathPartIndex];
                if (!pathPart.Equals(headerPathPart))
                    break;
            }

            List<string> splittedRelPathList = new List<string>();

            int pathDiffLength = splittedPath.Length - firstDifferentPathPartIndex;
            if (pathDiffLength > 1)
            {
                for (int i = 0; i < pathDiffLength - 1; i++)
                    splittedRelPathList.Add("..");
            }

            for (int i = 0; i < splittedHeaderPath.Length - firstDifferentPathPartIndex; i++)
                splittedRelPathList.Add(splittedHeaderPath[i + firstDifferentPathPartIndex]);

            //TODO: remove test code
            var result = string.Join("/", splittedRelPathList);

            return result;
        }
    }
}
