/* XMRig
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2017-2018 XMR-Stak    <https://github.com/fireice-uk>, <https://github.com/psychocrypt>
 * Copyright 2018      Lee Clagett <https://github.com/vtnerd>
 * Copyright 2018-2020 SChernykh   <https://github.com/SChernykh>
 * Copyright 2016-2020 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include <cstdlib>
#include <uv.h>


#include "App.h"
#include "backend/cpu/Cpu.h"
#include "base/io/Console.h"
#include "base/io/log/Log.h"
#include "base/io/log/Tags.h"
#include "base/io/Signals.h"
#include "base/kernel/Platform.h"
#include "core/config/Config.h"
#include "core/Controller.h"
#include "Summary.h"
#include "version.h"


xmrig::App::App(Process *process)
{
    m_controller = new Controller(process);
}


xmrig::App::~App()
{
    Cpu::release();

    delete m_signals;
    delete m_console;
    delete m_controller;
}


int xmrig::App::exec()
{
	printf("\n======APP::exec()======\n");
    if (!m_controller->isReady()) {
        LOG_EMERG("no valid configuration found, try https://xmrig.com/wizard");
		printf("APP::exec: ===no valid config found===\n");
        return 2;
    }

	printf("APP::exec: m_signals = new Signals()\n");
    m_signals = new Signals(this);

    int rc = 0;
	printf("APP::exec: int rc = %i\n", rc);
    if (background(rc)) {
		printf("APP::exec: ===background(rc), returning===\n");
        return rc;
    }

    rc = m_controller->init();
	printf("APP::exec: rc = m_controller->init() rc = %i\n", rc);
    if (rc != 0) {
		printf("==APP::exec: rc != 0, returning===\n");
        return rc;
    }

    if (!m_controller->isBackground()) {
        m_console = new Console(this);
		printf("APP::exec: if(!m_controller->isBackground) m_console = new Console(this)\n");
    }

    Summary::print(m_controller);
	printf("APP::exec: Summary::print(m_controller)\n");

    if (m_controller->config()->isDryRun()) {
        LOG_NOTICE("%s " WHITE_BOLD("OK"), Tags::config());
		printf("APP::exec: ==if(m_controller->config()->isDryRun(), returning 0===\n");
        return 0;
    }

	printf("APP::exec: m_controller->start()\n");
    m_controller->start();

	printf("APP::exec: rc = uv_run(uv_default_loop(), UV_RUN_DEFAULT)\n");
    rc = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	printf("APP::exec: rc = %i\n", rc);
    uv_loop_close(uv_default_loop());
	printf("APP::exec: uv_loop_close(uv_default_loop())\n");

	printf("APP::exec: ===return rc===\n");
    return rc;
}


void xmrig::App::onConsoleCommand(char command)
{
    if (command == 3) {
        LOG_WARN("%s " YELLOW("Ctrl+C received, exiting"), Tags::signal());
        close();
    }
    else {
        m_controller->execCommand(command);
    }
}


void xmrig::App::onSignal(int signum)
{
    switch (signum)
    {
    case SIGHUP:
        LOG_WARN("%s " YELLOW("SIGHUP received, exiting"), Tags::signal());
        break;

    case SIGTERM:
        LOG_WARN("%s " YELLOW("SIGTERM received, exiting"), Tags::signal());
        break;

    case SIGINT:
        LOG_WARN("%s " YELLOW("SIGINT received, exiting"), Tags::signal());
        break;

    default:
        return;
    }

    close();
}


void xmrig::App::close()
{
    m_signals->stop();

    if (m_console) {
        m_console->stop();
    }

    m_controller->stop();

    Log::destroy();
}
