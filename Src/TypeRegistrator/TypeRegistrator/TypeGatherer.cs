using System;
using System.Collections.Generic;
using System.IO;

namespace TypeRegistrator
{
    class TypeGatherer
    {
        public void GatherRegisteredTypes(string sourceDirectory, string setMacro, string outputFile, HashSet<string> headers, Dictionary<string, List<string>> types, bool gatherWithFields)
        {
            string[] allfiles = Directory.GetFiles(sourceDirectory, "*.h", SearchOption.AllDirectories);

            for (int i = 0; i < allfiles.Length; i++)
            {
                string file = allfiles[i].Replace('\\', '/');
                if (file.Equals(outputFile))
                    continue;

                //TODO: ignore comments near setMacro and it usages in comments
                if (GatherFromSource(file, setMacro, types, gatherWithFields))
                    headers.Add(GetRelativeHeaderPath(outputFile, file));
            }
        }

        private bool GatherFromSource(string file, string tag, Dictionary<string, List<string>> types, bool gatherWithFields)
        {
            var src = File.ReadAllText(file);

            bool anyTypeGathered = false;

            for (int tagIndex = 0; ; tagIndex += tag.Length)
            {
                tagIndex = src.IndexOf(tag, tagIndex);
                if (tagIndex == -1)
                    break;

                if (tagIndex > 7 && src.Substring(tagIndex - 8, 8).Equals("#define ")) //length of "#define " == 8
                    continue;

                anyTypeGathered = true;

                int typeStartIndex = tagIndex + tag.Length + 1;
                int typeEndIndex = GetTypeEndIndex(src, typeStartIndex);

                string type = src.Substring(typeStartIndex, typeEndIndex - typeStartIndex);
                if (!types.TryAdd(type, new List<string>()))
                {
                    Console.WriteLine("Error in source code. Type registered more than once");
                    Environment.Exit(2);
                }
                else if (gatherWithFields)
                    GatherTypeFields(src, typeEndIndex, types[type]);
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

        private char[] whiteSpaceChars = new char[] { ' ', '\t', '\n', '\r' };
        private void GatherTypeFields(string src, int typeBodyStartIndex, List<string> fields)
        {
            int typeBodyEndIndex = src.IndexOf('}', typeBodyStartIndex);

            int semicolonIndex = src.IndexOf(';', typeBodyStartIndex);
            while (semicolonIndex < typeBodyEndIndex)
            {
                int fieldStartIndex = src.LastIndexOfAny(whiteSpaceChars, semicolonIndex) + 1;
                fields.Add(src.Substring(fieldStartIndex, semicolonIndex - fieldStartIndex));
                semicolonIndex = src.IndexOf(';', semicolonIndex + 1);
            }
        }

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

            return string.Join("/", splittedRelPathList);
        }
    }
}
