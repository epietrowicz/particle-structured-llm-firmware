# Particle Structured LLM Firmware

This repository contains the firmware component of the Structured LLM project. It is intended to work alongside of the [Structured LLM App repository](https://github.com/epietrowicz/particle-structured-llm-app).

The Structured LLM project serves as a demo of how you might use a local LLM running on a Raspberry Pi to translate a voice command into an updated system state. 

For example, you might be tracking the state for an environmental controller:
```python
{ "temperature": 10, "fan": 0, "lights": 20, "window": 0 }
```

The LLM will respond to a voice command such as "it's too cold in here" with an updated state such as:
```python
{ "temperature": 30, "fan": 0, "lights": 20, "window": 0 }
```

The system state is then forwarded to a [Particle Ledger](https://www.particle.io/ledger-and-logic/) instance over LTE to be fetched by an end application. 