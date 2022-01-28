import json
import sys
from newsched import gr, blocks

class FlowgraphFromJson(gr.flowgraph):
    def __init__(self, data):
        """
        the json input is expected to have the following form
        blocks:
          name:
            {block: copy, args: []}
        connections:
            [name, port, name, port]
        """
        self._blocks = {}
        for name, info in data["blocks"].items():
            self._blocks[name] =  getattr(blocks, info["block"])(*info["args"])
        for out_name, out_port, in_name, in_port in data["connections"]:
            self.connect(self._blocks[out_name], out_port, self._blocks[in_name], in_port)

def main():
    with open(sys.argv[1]) as fid:
        fg = FlowgraphFromJson(json.load(fid))

    fg.run()

if __name__ == "__main__":
    main()        