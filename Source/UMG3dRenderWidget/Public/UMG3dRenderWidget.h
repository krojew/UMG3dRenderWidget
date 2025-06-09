// Copyright 2025 Kamil Rojewski. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FUMG3dRenderWidgetModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
