# llm-connector

![GitHub Workflow Status](https://img.shields.io/github/workflow/status/neuralmimicry/llm-connector/LLM-Connector)
[![Flattr this git repo](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=neuralmimicry&url=https://github.com/neuralmimicry/llm-connector&title=LLM-Connector&language=&tags=github&category=software)

Have conversations between multiple LLMs

With all the noise around Generative AI, it's relevant to have a system available to connect to many of them at once. This is the goal of LLM-Connector.
Why have one AI when you can have the input from many? Of course, you will need to form a consolidated answer and weed-out hallucinations, but that's the fun part.

Tuning, so that you do not need to keep setting a context for domain specific knowledge, is a goal of this project. This is parameterised so that the user can select
the area of domain knowledge at the start of the conversation.

Note: Crow and Asio include folders have been added to the project.

## Installation

```bash
git clone git@github.com:neuralmimicry/llm-connector.git
cd llm-connector
mkdir build
cd build
cmake ..
```

## Usage

```bash
./llm-connector
```

## Contributing
Feel free to contribute. We learn by doing.

## Sponsorship
Thanks to [Naska.AI](https://naska.ai) for sponsoring this project through a single-day (12-May-2023) hackathon to get things started.
