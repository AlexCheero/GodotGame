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
            if (args[0].Equals("clear"))
            {
                Console.WriteLine("clear header");
                string outFile = GetFixedPath(args[1]);
                File.WriteAllText(outFile, HeaderAssembler.HEADER_TEMPLATE);
                return;
            }

            string setMacro = args[0];
            string getMacro = args[1];
            
            string sourceDirectory = GetFixedPath(args[2]);
            string outputFile = GetFixedPath(args[3]);

            bool gatherWithFields = bool.Parse(args[4]);

            string declareMetaMacro = args.Length > 5 ? args[5] : string.Empty;

            Console.WriteLine("registering " + setMacro + " types, with " + getMacro + (gatherWithFields ? " with fields" : " without fields") +
                ", " +(declareMetaMacro.Length > 0 ? "with meta" : "without meta"));

            var headers = new HashSet<string>();
            var types = new Dictionary<string, List<string>>();

            new TypeGatherer().GatherRegisteredTypes(sourceDirectory, setMacro, outputFile, headers, types, gatherWithFields);

            if (types.Count > 0)
            {
                string output = new HeaderAssembler().GetHeaderSource(outputFile, headers, types, getMacro, gatherWithFields, declareMetaMacro);
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
