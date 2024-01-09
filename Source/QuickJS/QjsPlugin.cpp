/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 * Copyright (c) 2019-2023 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "QjsPlugin.h"
#include <RmlUi/Core/Core.h>
#include "QjsDocumentElementInstancer.h"
//#include "QjsEventListenerInstancer.h"

#include "RmlUi.h"
#include <RmlUi/Core/Factory.h>
#include <RmlUi/Core/FileInterface.h>
#include <RmlUi/Core/Log.h>

//#include <RmlUi/QuickJS/Qjs.h>
//#include <RmlUi/QuickJS/QjsType.h>
//#include <RmlUi/QuickJS/Utilities.h>

/*
// the types I made
#include "Colourb.h"
#include "Colourf.h"
#include "Context.h"
#include "ContextDocumentsProxy.h"
#include "Document.h"
#include "Element.h"
#include "ElementAttributesProxy.h"
#include "ElementChildNodesProxy.h"
#include "ElementInstancer.h"
#include "ElementStyleProxy.h"
#include "ElementText.h"
#include "Event.h"
#include "EventParametersProxy.h"
#include "GlobalLuaFunctions.h"
#include "Log.h"
#include "RmlUiContextsProxy.h"
#include "Vector2f.h"
#include "Vector2i.h"

// Control types
#include "Elements/ElementForm.h"
#include "Elements/ElementFormControl.h"
#include "Elements/ElementFormControlInput.h"
#include "Elements/ElementFormControlSelect.h"
#include "Elements/ElementFormControlTextArea.h"
#include "Elements/ElementTabSet.h"
#include "Elements/SelectOptionsProxy.h"
*/



namespace Rml {
namespace Qjs {


void Initialise()
{
	Initialise(nullptr);
}

void Initialise(JSContext* ctx )
{
	::Rml::RegisterPlugin(new QjsPlugin(ctx));
}



static JSRuntime* g_runtime = nullptr;
static JSContext* g_context = nullptr;

/** 
 * This will populate the global Lua table with all of the Lua core types by calling LuaType<T>::Register
 * @remark This is called automatically by LuaPlugin::OnInitialise(). 
 **/

static void RegisterTypes( JSContext* );


QjsPlugin::QjsPlugin(JSContext* ctx)
{
	RMLUI_ASSERT(g_context == nullptr);
	g_context = ctx;
}

int QjsPlugin::GetEventClasses()
{
	return EVT_BASIC;
}

void QjsPlugin::OnInitialise()
{
	if( g_runtime == nullptr ) {
		g_runtime = JS_NewRuntime( );
	}

	if (g_context == nullptr)
	{
		Log::Message(Log::LT_INFO, "Loading Lua plugin using a new Lua state.");
		g_context = JS_NewContext( g_runtime );
		//luaL_openlibs(g_context);
		owns_qjs_state = true;
	}
	else
	{
		Log::Message(Log::LT_INFO, "Loading Lua plugin using the provided Lua state.");
		owns_qjs_state = false;
	}

	RegisterTypes( g_context );

	qjs_document_element_instancer = new QjsDocumentElementInstancer();
	//lua_event_listener_instancer = new LuaEventListenerInstancer();
	Factory::RegisterElementInstancer("body", qjs_document_element_instancer);
	//Factory::RegisterEventListenerInstancer(lua_event_listener_instancer);
}

void QjsPlugin::OnShutdown()
{
	//delete lua_document_element_instancer;
	//delete lua_event_listener_instancer;
	//lua_document_element_instancer = nullptr;
	//lua_event_listener_instancer = nullptr;

	if (owns_qjs_state && g_context ) {
		JS_FreeContext( g_context );
	}

	g_context = nullptr;
	delete this;
}

static void RegisterTypes( JSContext* ctx )
{
	setupRmlUi( ctx );

	/*
	RMLUI_ASSERT(g_context);
	lua_State* L = g_context;

	LuaType<Vector2i>::Register(L);
	LuaType<Vector2f>::Register(L);
	LuaType<Colourf>::Register(L);
	LuaType<Colourb>::Register(L);
	LuaType<Log>::Register(L);
	LuaType<ElementStyleProxy>::Register(L);
	LuaType<Element>::Register(L);
	// things that inherit from Element
	LuaType<Document>::Register(L);
	LuaType<ElementText>::Register(L);
	LuaType<ElementPtr>::Register(L);
	LuaType<Event>::Register(L);
	LuaType<Context>::Register(L);
	LuaType<LuaRmlUi>::Register(L);
	LuaType<ElementInstancer>::Register(L);
	// Proxy tables
	LuaType<ContextDocumentsProxy>::Register(L);
	LuaType<EventParametersProxy>::Register(L);
	LuaType<ElementAttributesProxy>::Register(L);
	LuaType<ElementChildNodesProxy>::Register(L);
	LuaType<RmlUiContextsProxy>::Register(L);
	OverrideLuaGlobalFunctions(L);
	// push the global variable "rmlui" to use the "RmlUi" methods
	LuaRmlUiPushrmluiGlobal(L);

	// Control types
	LuaType<ElementForm>::Register(L);
	LuaType<ElementFormControl>::Register(L);
	// Inherits from ElementFormControl
	LuaType<ElementFormControlSelect>::Register(L);
	LuaType<ElementFormControlInput>::Register(L);
	LuaType<ElementFormControlTextArea>::Register(L);
	LuaType<ElementTabSet>::Register(L);
	// proxy tables
	LuaType<SelectOptionsProxy>::Register(L);
	*/
}

JSContext* QjsPlugin::GetQjsState()
{
	return g_context;
}

} // namespace Qjs
} // namespace Rml
