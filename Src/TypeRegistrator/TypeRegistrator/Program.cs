using System.IO;
using System.Collections.Generic;
using System;
using System.Linq;

namespace TypeRegistrator
{
    class Program
    {
        //TODO: add different modes: clear, reg tag, reg component etc.
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

            Console.WriteLine("registering " + setMacro + " types, with " + getMacro);
            
            string sourceDirectory = GetFixedPath(args[2]);
            string outputFile = GetFixedPath(args[3]);
            
            var headers = new HashSet<string>();
            var types = new HashSet<string>();

            new TypeGatherer().GatherRegisteredTypes(sourceDirectory, setMacro, outputFile, headers, types);

            if (types.Count > 0)
            {
                string output = new HeaderAssembler().GetHeaderSource(outputFile, headers, types, getMacro);
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
