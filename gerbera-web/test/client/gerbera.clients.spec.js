import {GerberaApp} from "../../../web/js/gerbera-app.module";
import {Clients} from '../../../web/js/gerbera-clients.module';
import mockConfig from './fixtures/config';
import clientsDataJson from './fixtures/clients-data';
import gerberaEmptyClients from './fixtures/clients-empty';

describe('Gerbera Clients', () => {
  let lsSpy;
  beforeEach(() => {
    fixture.setBase('test/client/fixtures');
    fixture.load('index.html');
    lsSpy = spyOn(window.localStorage, 'getItem').and.callFake((name) => {
        return;
    });
  });
  afterEach((done) => {
    fixture.cleanup();
    done();
  });

  describe('initialize()', () => {
   beforeEach(() => {
     GerberaApp.serverConfig = mockConfig.config;
   });

   it('clears the datagrid', async () => {
     await Clients.initialize();
     expect($('#clientgrid').text()).toBe('');
   });
  });
  describe('loadItems()', () => {
    beforeEach(() => {
      GerberaApp.serverConfig = {};
      spyOn(GerberaApp, 'getType').and.returnValue('clients');
    });

    it('does not load items if response is failure', () => {
      clientsDataJson.success = false;
      Clients.loadItems(clientsDataJson);
      expect($('#clientgrid').find('tr').length).toEqual(0);
      clientsDataJson.success = true;
    });

    it('does not create heading if result is empty', () => {
      Clients.loadItems(gerberaEmptyClients);
      expect($('#clientgrid').find('tr').length).toEqual(2);
    });
    
    it('loads the response as items in the datagrid', () => {
      clientsDataJson.success = true;
      Clients.loadItems(clientsDataJson);
      expect($('#clientgrid').find('tr').length).toEqual(4);
      clientsDataJson.success = true;
    });
  });
 });
