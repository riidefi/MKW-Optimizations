# MKW-Optimizations
This project extends Mario Kart Wii to allow more efficiency for custom content!

## New features are embedded in the course KMP
All additional features are added to a new section of the KMP format (RII0). The section is designed to be highly extendable. New features are gracefully ignored by older versions, and old features are fully supported by new versions.

## Trigger-Event System
The current system has two important concepts: triggers and events. Triggers are registered to special events (e.g., entering or exiting a certain checkpoint region) and may call one event. While the only actionable event currently is showing and hiding certain polygons in the course model, an chain events allow a single action to have multiple effects.
