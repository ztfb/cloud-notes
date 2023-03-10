package com.cloudnotes.service.service.impl;

import com.cloudnotes.service.common.Result;
import com.cloudnotes.service.entity.Note;
import com.cloudnotes.service.entity.ShareRelation;
import com.cloudnotes.service.entity.TextBlock;
import com.cloudnotes.service.mapper.NoteMapper;
import com.cloudnotes.service.mapper.ShareNoteMapper;
import com.cloudnotes.service.mapper.TextBlockMapper;
import com.cloudnotes.service.service.NoteService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class NoteServiceImpl implements NoteService {
    private class data{
        public Integer id;
        public String shareCode;
    }
    @Autowired
    NoteMapper noteMapper;
    @Autowired
    TextBlockMapper textBlockMapper;
    @Autowired
    ShareNoteMapper shareNoteMapper;
    @Autowired
    BCryptPasswordEncoder encoder;
    public class info{
        public Integer id;
        public Integer nid;
        public String name;
    }
    @Override
    public List<Note> find(Integer cid) {
        return noteMapper.findByCatalogId(cid);
    }

    @Override
    public Result create(Note note) {
        Note temp=noteMapper.save(note);
        temp.setShareCode(encoder.encode(String.valueOf(temp.getId())));
        noteMapper.save(temp);
        data d=new data();
        d.id=temp.getId();d.shareCode=temp.getShareCode();
        TextBlock textBlock=new TextBlock();
        textBlock.setNoteId(temp.getId());
        textBlock.setUserId(temp.getUserId());
        textBlockMapper.save(textBlock);
        return new Result("0","保存成功",d);
    }

    @Override
    public Result delete(Integer id) {
        noteMapper.deleteById(id);
        List<TextBlock> textBlocks=textBlockMapper.findTextBlocksByNoteId(id);
        for(TextBlock textBlock:textBlocks){
            textBlockMapper.deleteById(textBlock.getId());
        }
        List<ShareRelation> relations=shareNoteMapper.findShareRelationsByNoteId(id);
        for(ShareRelation relation:relations){
            shareNoteMapper.deleteById(relation.getId());
        }
        return new Result("0","删除成功",null);
    }

    @Override
    public Result update(Note note) {
        Note temp=noteMapper.findNoteById(note.getId());
        temp.setName(note.getName());
        temp.setAuthor(note.getAuthor());
        temp.setLabel(note.getLabel());
        noteMapper.save(temp);
        return new Result("0","修改成功",null);
    }

    @Override
    public Result findText(Integer nid) {
        return new Result("0","查询成功",noteMapper.findText(nid));
    }

    @Override
    public Result updateText(TextBlock textBlock) {
        TextBlock temp=textBlockMapper.findTextBlockByNoteId(textBlock.getNoteId());
        temp.setText(textBlock.getText());
        textBlockMapper.save(temp);
        return new Result("0","修改成功",null);
    }

    @Override
    public Result updatePower(Note note) {
        Note temp=noteMapper.findNoteById(note.getId());
        temp.setDefaultPower(note.getDefaultPower());
        noteMapper.save(temp);
        List<ShareRelation> relations=shareNoteMapper.findShareRelationsByNoteId(temp.getId());
        for(ShareRelation relation:relations){
            relation.setPower(temp.getDefaultPower());
            shareNoteMapper.save(relation);
        }
        return new Result("0","修改成功",null);
    }

    @Override
    public Result shareNew(Note note) {
        Integer sid=note.getUserId();
        String shareCode=note.getShareCode();
        Note temp=noteMapper.findByShareCode(shareCode);
        if(temp==null) return new Result("1","共享码不存在",null);
        else{
            ShareRelation relation=new ShareRelation();
            relation.setNoteId(temp.getId());
            relation.setUserId(temp.getUserId());
            relation.setPower(temp.getDefaultPower());
            relation.setShareId(sid);
            relation.setNoteName(temp.getName());
            ShareRelation temp2=shareNoteMapper.save(relation);
            info i=new info();
            i.id=temp2.getId();
            i.nid=temp.getId();
            i.name=temp.getName();
            return new Result("0","添加成功",i);
        }
    }
}
