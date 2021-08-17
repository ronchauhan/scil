# scil
Simple CFG based intermediate language to work with classical data flow and control flow related concepts.

The overall design is super simple. It's literally just a control flow graph that supports ONLY 64bit integers, explicit branches (no fall through), no procedures or calls at the moment.

There's no memory management for IR objects. The IR is meant to be generated from the text. This repo also contains a quick parser to help with that. The CFG can be dumped, be printed as parsable text, or emitted to a DOT file.

I find it suitable for trying out some analyses and transformations to gain a deeper understanding. Some todos would be to include memory operations, cleanup the IR implementation, define the grammar properly.

Here's how it looks like at the moment:

Parsable text
```
.bb0
  a = 34
  b = add a, 33
  c = a > b
  br c bb1 bb2

.bb1
  b = 99
  c = 98
  a = add b, c
  jmp bb2

.bb2
  a = 300
```

<br>

Dump
```
Block : bb0
Predecessors :
Successors : bb1 bb2
<RegDef> <Reg:a> <Imm:34>
<Add> <Reg:b> <Reg:a> <Imm:33>
<Gt> <Reg:c> <Reg:a> <Reg:b>
<Br> <Reg:c> <Label:bb1> <Label:bb2>

Block : bb1
Predecessors : bb0
Successors : bb2
<RegDef> <Reg:b> <Imm:99>
<RegDef> <Reg:c> <Imm:98>
<Add> <Reg:a> <Reg:b> <Reg:c>
<Jmp> <Label:bb2>

Block : bb2
Predecessors : bb0 bb1
Successors :
<RegDef> <Reg:a> <Imm:300>
```

<br>

Graph

<img src="https://gist.githubusercontent.com/ronchauhan/22190f9a08f7b2ca79f89ba8feba74ae/raw/19ee44c585e8ea393854927d13689babe87fb5fa/cfg.png">

<br>

Graph (simple)

<img src="https://gist.githubusercontent.com/ronchauhan/22190f9a08f7b2ca79f89ba8feba74ae/raw/19ee44c585e8ea393854927d13689babe87fb5fa/cfg-simple.png">

<br>

# License
Licensed under the MIT license.
