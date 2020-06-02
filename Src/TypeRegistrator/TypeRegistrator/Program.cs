using System.IO;
using System.Collections.Generic;
using System;
using System.Linq;

namespace TypeRegistrator
{
    class Program
    {
        static void Main(string[] args)
        {
            bool newOutput = bool.Parse(args[0]);

            string setMacro = args[1];
            string getMacro = args[2];
            
            string sourceDirectory = args[3];
            string outputFile = args[4];
            string registerationMacroMacroDefinitionFile = args[5];

            sourceDirectory = GetFixedPath(sourceDirectory);
            outputFile = GetFixedPath(outputFile);
            registerationMacroMacroDefinitionFile = GetFixedPath(registerationMacroMacroDefinitionFile);

            string[] fileExcludes = new string[] { outputFile, registerationMacroMacroDefinitionFile };

            var headers = new HashSet<string>();
            var types = new HashSet<string>();

            new TypeGatherer().GatherRegisteredTypes(sourceDirectory, setMacro, outputFile, fileExcludes, headers, types);

            if (types.Count > 0)
            {
                string output = new HeaderAssembler().GetHeaderSource(newOutput, outputFile, headers, types, getMacro);
                File.WriteAllText(outputFile, output);
            }
        }

        static string GetFixedPath(string path)
        {
            path = path.Replace('\\', '/');
            var splittedPath = path.Split('/');
            var fixedSplittedPath = new Stack<string>();

            foreach (var pathPart in splittedPath)
            {
                if (pathPart != "..")
                    fixedSplittedPath.Push(pathPart);
                else
                    fixedSplittedPath.Pop();
            }

            return string.Join("/", fixedSplittedPath.Reverse());
        }
    }
}
