#include "ModsLayer.hpp"
#include "SwelvyBG.hpp"
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include "GeodeStyle.hpp"

bool ModsLayer::init() {
    if (!CCLayer::init())
        return false;

    auto winSize = CCDirector::get()->getWinSize();
    
    this->addChild(SwelvyBG::create());
    
    auto backMenu = CCMenu::create();
    backMenu->setContentWidth(100.f);
    backMenu->setAnchorPoint({ .0f, .5f });
    
    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    auto backBtn = CCMenuItemSpriteExtra::create(
        backSpr, this, menu_selector(ModsLayer::onBack)
    );
    backMenu->addChild(backBtn);

    backMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    this->addChildAtPosition(backMenu, Anchor::TopLeft, ccp(12, -25), false);

    auto actionsMenu = CCMenu::create();
    actionsMenu->setContentHeight(200.f);
    actionsMenu->setAnchorPoint({ .5f, .0f });

    auto reloadSpr = CircleButtonSprite::create(
        CCSprite::createWithSpriteFrameName("reload.png"_spr),
        CircleBaseColor::DarkPurple,
        CircleBaseSize::Medium
    );
    reloadSpr->setScale(.8f);
    reloadSpr->setTopOffset(ccp(1, 0));
    auto reloadBtn = CCMenuItemSpriteExtra::create(
        reloadSpr, this, menu_selector(ModsLayer::onRefreshList)
    );
    actionsMenu->addChild(reloadBtn);

    actionsMenu->setLayout(
        ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    this->addChildAtPosition(actionsMenu, Anchor::BottomLeft, ccp(35, 12), false);

    m_frame = CCNode::create();
    m_frame->setAnchorPoint({ .5f, .5f });
    m_frame->setContentSize({ 380, 205 });

    auto frameBG = CCLayerColor::create(ColorProvider::get()->color("mod-list-bg"_spr));
    frameBG->setContentSize(m_frame->getContentSize());
    frameBG->ignoreAnchorPointForPosition(false);
    m_frame->addChildAtPosition(frameBG, Anchor::Center);

    auto tabsTop = CCSprite::createWithSpriteFrameName("mods-list-top.png"_spr);
    tabsTop->setAnchorPoint({ .5f, .0f });
    m_frame->addChildAtPosition(tabsTop, Anchor::Top, ccp(0, -2));

    auto tabsLeft = CCSprite::createWithSpriteFrameName("mods-list-side.png"_spr);
    tabsLeft->setScaleY(m_frame->getContentHeight() / tabsLeft->getContentHeight());
    m_frame->addChildAtPosition(tabsLeft, Anchor::Left, ccp(6, 0));

    auto tabsRight = CCSprite::createWithSpriteFrameName("mods-list-side.png"_spr);
    tabsRight->setFlipX(true);
    tabsRight->setScaleY(m_frame->getContentHeight() / tabsRight->getContentHeight());
    m_frame->addChildAtPosition(tabsRight, Anchor::Right, ccp(-6, 0));

    auto tabsBottom = CCSprite::createWithSpriteFrameName("mods-list-bottom.png"_spr);
    tabsBottom->setAnchorPoint({ .5f, 1.f });
    m_frame->addChildAtPosition(tabsBottom, Anchor::Bottom, ccp(0, 2));

    this->addChildAtPosition(m_frame, Anchor::Center, ccp(0, -10), false);

    auto mainTabs = CCMenu::create();
    mainTabs->setContentWidth(tabsTop->getContentWidth() - 45);
    mainTabs->setAnchorPoint({ .5f, .0f });
    mainTabs->setPosition(m_frame->convertToWorldSpace(tabsTop->getPosition() + ccp(0, 10)));

    for (auto item : std::initializer_list<std::tuple<const char*, const char*, ModListSource*>> {
        { "download.png"_spr, "Installed", InstalledModListSource::get(false) },
        { "GJ_timeIcon_001.png", "Updates", InstalledModListSource::get(true) },
        { "globe.png"_spr, "Download", ServerModListSource::get(ServerModListType::Download) },
        { "GJ_sTrendingIcon_001.png", "Trending", ServerModListSource::get(ServerModListType::Trending) },
        { "gj_folderBtn_001.png", "Mod Packs", ModPackListSource::get() },
    }) {
        auto btn = CCMenuItemSpriteExtra::create(
            GeodeTabSprite::create(std::get<0>(item), std::get<1>(item), 100),
            this, menu_selector(ModsLayer::onTab)
        );
        btn->setUserData(std::get<2>(item));
        mainTabs->addChild(btn);
        m_tabs.push_back(btn);
    }

    mainTabs->setLayout(RowLayout::create());
    this->addChild(mainTabs);

    // Actions

    auto listActionsMenu = CCMenu::create();
    listActionsMenu->setContentHeight(100);
    listActionsMenu->setAnchorPoint({ 1, 0 });
    listActionsMenu->setScale(.65f);

    auto bigSizeBtn = CCMenuItemSpriteExtra::create(
        GeodeSquareSprite::createWithSpriteFrameName("GJ_smallModeIcon_001.png", &m_bigView),
        this, menu_selector(ModsLayer::onBigView)
    );
    listActionsMenu->addChild(bigSizeBtn);

    auto searchBtn = CCMenuItemSpriteExtra::create(
        GeodeSquareSprite::createWithSpriteFrameName("search.png"_spr, &m_showSearch),
        this, menu_selector(ModsLayer::onSearch)
    );
    listActionsMenu->addChild(searchBtn);

    listActionsMenu->setLayout(ColumnLayout::create());
    m_frame->addChildAtPosition(listActionsMenu, Anchor::Left, ccp(-5, 25));

    auto restartMenu = CCMenu::create();
    restartMenu->setContentWidth(200.f);
    restartMenu->setAnchorPoint({ .5f, 1.f });
    restartMenu->setScale(.7f);

    m_restartBtn = CCMenuItemSpriteExtra::create(
        createGeodeButton("Restart Now"),
        this, menu_selector(ModsLayer::onRestart)
    );
    restartMenu->addChild(m_restartBtn);

    restartMenu->setLayout(RowLayout::create());
    m_frame->addChildAtPosition(restartMenu, Anchor::Bottom, ccp(0, 0));

    m_pageMenu = CCMenu::create();
    m_pageMenu->setContentWidth(200.f);
    m_pageMenu->setAnchorPoint({ 1.f, 1.f });
    m_pageMenu->setScale(.65f);

    m_pageLabel = CCLabelBMFont::create("", "goldFont.fnt");
    m_pageLabel->setAnchorPoint({ .5f, 1.f });
    m_pageMenu->addChild(m_pageLabel);

    m_goToPageBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("gj_navDotBtn_on_001.png"),
        this, menu_selector(ModsLayer::onGoToPage)
    );
    m_pageMenu->addChild(m_goToPageBtn);

    m_pageMenu->setLayout(
        RowLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
    );
    this->addChildAtPosition(m_pageMenu, Anchor::TopRight, ccp(-5, -5), false);

    // Go to installed mods list
    this->gotoTab(InstalledModListSource::get(false));

    this->setKeypadEnabled(true);
    cocos::handleTouchPriority(this, true);

    this->updateState();

    // The overall mods layer only cares about page number updates
    m_updateStateListener.setFilter(UpdateModListStateFilter(UpdatePageNumberState()));
    m_updateStateListener.bind([this](auto) { this->updateState(); });

    return true;
}

void ModsLayer::gotoTab(ModListSource* src) {
    // Update selected tab
    for (auto tab : m_tabs) {
        auto selected = tab->getUserData() == static_cast<void*>(src);
        static_cast<GeodeTabSprite*>(tab->getNormalImage())->select(selected);
        tab->setEnabled(!selected);
    }

    // Remove current list from UI (it's Ref'd so it stays in memory)
    if (m_currentSource) {
        m_lists.at(m_currentSource)->removeFromParent();
    }

    // Lazily create new list and add it to UI
    if (!m_lists.contains(src)) {
        auto list = ModList::create(src, m_frame->getContentSize() - ccp(30, 0));
        list->setPosition(m_frame->getPosition());
        this->addChild(list);
        m_lists.emplace(src, list);
    }
    // Add list to UI
    else {
        this->addChild(m_lists.at(src));
    }

    // Update current source
    m_currentSource = src;

    // Update the state of the current list
    m_lists.at(m_currentSource)->updateSize(m_bigView);
    m_lists.at(m_currentSource)->activateSearch(m_showSearch);
    m_lists.at(m_currentSource)->updateState();
}

void ModsLayer::keyBackClicked() {
    this->onBack(nullptr);
}

void ModsLayer::setTextPopupClosed(SetTextPopup* popup, gd::string value) {
    if (popup->getID() == "go-to-page"_spr) {
        if (auto res = numFromString<size_t>(value)) {
            size_t num = res.unwrap();
            // The page indices are 0-based but people think in 1-based
            if (num > 0) num -= 1;
            if (m_currentSource) {
                m_lists.at(m_currentSource)->gotoPage(num);
            }
        }
    }
}

void ModsLayer::updateState() {
    // Show current page number if the current source has total page count loaded
    if (m_currentSource && m_currentSource->getPageCount()) {
        auto page = m_lists.at(m_currentSource)->getPage() + 1;
        auto count = m_currentSource->getPageCount().value();
        auto total = m_currentSource->getItemCount().value();

        // Set the page count string
        auto fmt = fmt::format("Page {}/{} (Total {})", page, count, total);
        m_pageLabel->setString(fmt.c_str());

        // Make page menu visible
        m_pageMenu->setVisible(true);
        m_pageMenu->updateLayout();
    }
    // Hide page menu otherwise
    else {
        m_pageMenu->setVisible(false);
    }

    // Update visibility of the restart button
    m_restartBtn->setVisible(false);
    for (auto& [src, _] : m_lists) {
        if (src->wantsRestart()) {
            m_restartBtn->setVisible(true);
        }
    }
}

void ModsLayer::onTab(CCObject* sender) {
    this->gotoTab(static_cast<ModListSource*>(static_cast<CCNode*>(sender)->getUserData()));
}

void ModsLayer::onRefreshList(CCObject*) {
    if (m_currentSource) {
        m_lists.at(m_currentSource)->reloadPage();
    }
}

void ModsLayer::onBack(CCObject*) {
    CCDirector::get()->replaceScene(CCTransitionFade::create(.5f, MenuLayer::scene(false)));

    // To avoid memory overloading, clear caches after leaving the layer
    server::clearServerCaches(true);
    clearAllModListSourceCaches();
}

void ModsLayer::onGoToPage(CCObject*) {
    auto popup = SetTextPopup::create("", "Page", 5, "Go to Page", "OK", true, 60.f);
    popup->m_delegate = this;
    popup->m_input->m_allowedChars = getCommonFilterAllowedChars(CommonFilter::Uint);
    popup->setID("go-to-page"_spr);
    popup->show();
}

void ModsLayer::onBigView(CCObject*) {
    m_bigView = !m_bigView;
    // Make sure to avoid a crash
    if (m_currentSource) {
        m_lists.at(m_currentSource)->updateSize(m_bigView);
    }
}

void ModsLayer::onSearch(CCObject*) {
    m_showSearch = !m_showSearch;
    // Make sure to avoid a crash
    if (m_currentSource) {
        m_lists.at(m_currentSource)->activateSearch(m_showSearch);
    }
}

void ModsLayer::onRestart(CCObject*) {
    // Update button state to let user know it's restarting but it might take a bit
    m_restartBtn->setEnabled(false);
    static_cast<ButtonSprite*>(m_restartBtn->getNormalImage())->setString("Restarting...");

    // Actually restart
    game::restart();
}

ModsLayer* ModsLayer::create() {
    auto ret = new ModsLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ModsLayer* ModsLayer::scene() {
    auto scene = CCScene::create();
    auto layer = ModsLayer::create();
    scene->addChild(layer);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, scene));
    return layer;
}

server::ServerRequest<std::vector<std::string>> ModsLayer::checkInstalledModsForUpdates() {
    return server::checkAllUpdates().map([](auto* result) -> Result<std::vector<std::string>, server::ServerError> {
        if (result->isOk()) {
            std::vector<std::string> updatesFound;
            for (auto& update : result->unwrap()) {
                if (update.hasUpdateForInstalledMod()) {
                    updatesFound.push_back(update.id);
                }
            }
            return Ok(updatesFound);
        }
        return Err(result->unwrapErr());
    });
}
