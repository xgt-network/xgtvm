```sh
sudo apt-get install -y build-essential cmake jq
```

```sh
brew install cmake jq
```

To manage the project more easily, if you have Ruby installed:

```sh
rake -T         # list tasks that can be run
rake clean      # removes build artifacts
rake configure  # configures the project
rake make       # builds the project
rake run        # runs the project
rake            # configures and makes the project
```

Example finished machine data:

```json
{
  "returnValue": "",
  "gasUsed": "9",
  "finalState": {
    "pc": 5,
    "opcode": {
      "name": "STOP",
      "fee": 0,
      "isAsync": false
    },
    "stack": ["8"]
  }
}
```
